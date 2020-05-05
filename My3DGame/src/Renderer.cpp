// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Renderer.h"
#include <iostream>
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "UIScreen.h"
#include "Game.h"
#include "GBuffer.h"
#include "PointLightComponent.h"

Renderer::Renderer(Game* game) :mGame(game), mCurrentShader(nullptr), mMirrorBuffer(0), mMirrorTexture(nullptr), mGBuffer(nullptr)
{
	//
}
Renderer::~Renderer()
{
	//
}
bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
	// Set OpenGL attributes
	// Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	mWindow = SDL_CreateWindow("<Window Title>", 100, 100, static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	// Create an OpenGL context
	mContext = SDL_GL_CreateContext(mWindow);
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();
	// Make sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}
	// Create quad for drawing sprites
	CreateSpriteVerts();
	// Create render target for mirror
	if (!CreateMirrorTarget())
	{
		SDL_Log("Failed to create render target for mirror.");
		return false;
	}
	// Create G-buffer
	mGBuffer = new GBuffer();
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	if (!mGBuffer->Create(width, height))
	{
		SDL_Log("Failed to create G-buffer.");
		return false;
	}
	// Load point light mesh
	mPointLightMesh = GetMesh("Assets/PointLight.gpmesh");
	return true;
}
void Renderer::Shutdown()
{
	// Get rid of any render target textures, if they exist
	if (mMirrorTexture != nullptr)
	{
		glDeleteFramebuffers(1, &mMirrorBuffer);
		mMirrorTexture->Unload();
		delete mMirrorTexture;
	}
	// Get rid of G-buffer
	if (mGBuffer != nullptr)
	{
		mGBuffer->Destroy();
		delete mGBuffer;
	}
	// Delete point lights
	while (!mPointLights.empty())
	{
		delete mPointLights.back();
	}
	delete mSpriteVerts;
	// Unset mCurrentShader pointer so you don't delete any actual shaders
	mCurrentShader = nullptr;
	delete mCurrentShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}
void Renderer::UnloadData()
{
	// Destroy meshShaders
	// Note: shaders and MeshComponents get deleted somewhere else
	mMeshShaders.clear();
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();
	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
	// Destroy shaders
	for (auto i : mShaders)
	{
		i.second->Unload();
		delete i.second;
	}
	mShaders.clear();
	
}
void Renderer::Draw()
{
	// Draw to the mirror texture first
	Draw3DScene(mMirrorBuffer, mMirrorView, mProjection);
	// Draw the 3D scene to the G-buffer
	Draw3DScene(mGBuffer->GetBufferID(), mView, mProjection, false);
	// Set the frame buffer back to zero (screen's frame buffer)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw from the GBuffer
	DrawFromGBuffer();
	// Draw all sprite components
	// Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	// Set shader/VAO as active
	SetShader("Sprite");
	mSpriteVerts->SetActive();
	for (auto sprite : mSprites)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(mCurrentShader);
		}
	}
	// Draw any UI screens
	for (auto ui : mGame->GetUIStack())
	{
		ui->Draw(mCurrentShader);
	}
	// Swap the buffers
	SDL_GL_SwapWindow(mWindow);
}
void Renderer::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto i = mSprites.begin();
	for (; i != mSprites.end(); ++i)
	{
		if (myDrawOrder < (*i)->GetDrawOrder())
		{
			break;
		}
	}
	// Inserts element before position of iterator
	mSprites.insert(i, sprite);
}
void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	auto i = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(i);
}
void Renderer::AddMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshComps.emplace_back(sk);
	}
	else
	{
		mMeshComps.emplace_back(mesh);
	}
}
void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto i = std::find(mSkeletalMeshComps.begin(), mSkeletalMeshComps.end(), sk);
		mSkeletalMeshComps.erase(i);
	}
	else
	{
		auto i = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
		mMeshComps.erase(i);
	}
	UnlinkMesh(mesh);
}
void Renderer::AddPointLight(PointLightComponent* light)
{
	mPointLights.emplace_back(light);
}
void Renderer::RemovePointLight(PointLightComponent* light)
{
	auto i = find(mPointLights.begin(), mPointLights.end(), light);
	mPointLights.erase(i);
}
Texture* Renderer::GetTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto i = mTextures.find(fileName);
	if (i != mTextures.end())
	{
		tex = i->second;
	}
	else
	{
		tex = new Texture();
		if (tex->Load(fileName))
		{
			mTextures.emplace(fileName, tex);
		}
		else
		{
			delete tex;
			tex = nullptr;
		}
	}
	return tex;
}
void Renderer::RemoveTexture(const std::string& name)
{
	auto it = mTextures.find(name);
	if (it != mTextures.end())
	{
		mTextures.erase(it);
	}
}
void Renderer::RemoveTexture(Texture* tex)
{
	auto it = mTextures.begin();
	while (it != mTextures.end())
	{
		if (it->second == tex)
		{
			break;
		}
		it++;
	}
	if (it != mTextures.end())
	{
		mTextures.erase(it);
	}
}
Mesh* Renderer::GetMesh(const std::string & fileName)
{
	Mesh* m = nullptr;
	auto i = mMeshes.find(fileName);
	if (i != mMeshes.end())
	{
		m = i->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}
void Renderer::RemoveMesh(const std::string& name)
{
	auto it = mMeshes.find(name);
	if (it != mMeshes.end())
	{
		mMeshes.erase(it);
	}
}
void Renderer::RemoveMesh(Mesh* mesh)
{
	auto it = mMeshes.begin();
	while (it != mMeshes.end())
	{
		if (it->second == mesh)
		{
			break;
		}
		it++;
	}
	if (it != mMeshes.end())
	{
		mMeshes.erase(it);
	}
}
Shader* Renderer::CreateShader(const std::string& name, const std::string& fileName)
{
	return CreateShader(name, fileName+".vert", fileName+".frag");
}
Shader* Renderer::CreateShader(const std::string& name, const std::string & vertexFile, const std::string & fragmentFile)
{
	Shader* s = new Shader(name);
	if (s->Load(vertexFile, fragmentFile))
	{
		mShaders.emplace(s->GetName(), s);
		s->SetActive();
	}
	else
	{
		delete s;
		s = nullptr;
	}
	return s;
}
Shader* Renderer::GetShader(const std::string& name)
{
	auto i = mShaders.find(name);
	if (i != mShaders.end())
	{
		return i->second;
	}
	else
	{
		std::cerr << "Failed to get shader \"" << name << "\"" << std::endl;
		return nullptr;
	}
}
void Renderer::SetShader(const std::string& name)
{
	Shader* s = GetShader(name);
	mCurrentShader = s;
	s->SetActive();	
}
void Renderer::RemoveShader(const std::string& name)
{
	auto it = mShaders.find(name);
	if (it != mShaders.end())
	{
		mShaders.erase(it);
	}
}
void Renderer::RemoveShader(Shader* shader)
{
	auto it = mShaders.begin();
	while (it != mShaders.end())
	{
		if (it->second == shader)
		{
			break;
		}
		it++;
	}
	if (it != mShaders.end())
	{
		mShaders.erase(it);
	}
}
std::vector<MeshComponent*>* Renderer::LinkMeshToShader(MeshComponent* mesh, Shader* shader)
{
	std::vector<MeshComponent*>* meshVector = GetMeshShader(shader);
	if (meshVector == nullptr)
	{
		meshVector = new std::vector<MeshComponent*>();
		meshVector->emplace_back(mesh);
		mMeshShaders.emplace(shader, meshVector);
	}
	else
	{
		meshVector->emplace_back(mesh);
	}
	return meshVector;
}
bool Renderer::LinkMeshesToShader(Shader* shader, std::vector<MeshComponent*>* meshVector)
{
	if (GetMeshShader(shader) != nullptr)
	{
		return false;
	}
	mMeshShaders.emplace(shader, meshVector);
	return true;
}
std::vector<MeshComponent*>* Renderer::GetMeshShader(Shader* shader)
{
	std::vector<MeshComponent*>* meshVector = nullptr;
	auto it = mMeshShaders.find(shader);
	if (it != mMeshShaders.end())
	{
		meshVector = it->second;
	}
	return meshVector;
}
Shader* Renderer::GetShaderFromMesh(MeshComponent* mesh)
{
	auto it = mMeshShaders.begin();
	while (it != mMeshShaders.end())
	{
		std::vector<MeshComponent*>* meshVector = it->second;
		auto i = find(meshVector->begin(), meshVector->end(), mesh);
		if (i != meshVector->end())
		{
			return it->first;
		}
		it++;
	}
	return nullptr;
}
void Renderer::UnlinkShader(Shader* shader)
{
	auto it = mMeshShaders.find(shader);
	if (it != mMeshShaders.end())
	{
		mMeshShaders.erase(it);
	}
}
void Renderer::UnlinkMesh(MeshComponent* mesh)
{
	auto it = mMeshShaders.begin();
	while (it != mMeshShaders.end())
	{
		std::vector<MeshComponent*>* meshVector = it->second;
		auto i = find(meshVector->begin(), meshVector->end(), mesh);
		if (i != meshVector->end())
		{
			meshVector->erase(i);
			break;
		}
		it++;
	}
}
void Renderer::UnlinkMeshes(std::vector<MeshComponent*>* meshVector)
{
	auto it = mMeshShaders.begin();
	while (it != mMeshShaders.end())
	{
		if (it->second == meshVector)
		{
			break;
		}
		it++;
	}
	if (it != mMeshShaders.end())
	{
		mMeshShaders.erase(it);
	}
}
void Renderer::Draw3DScene(unsigned int framebuffer, const matrix4& view, const matrix4& proj, bool lit)
{
	// Set the current frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Clear color buffer/depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw mesh components
	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	for (auto it = mMeshShaders.begin(); it != mMeshShaders.end(); it++)
	{
		// Set the specific mesh shader as active
		mCurrentShader = it->first;
		mCurrentShader->SetActive(); 
		// Update view-projection matrix
		mCurrentShader->SetMatrixUniform("uViewProj", view * proj);
		// Update lighting uniforms
		if (lit)
		{
			SetLightUniforms(mCurrentShader, view);
		}
		std::vector<MeshComponent*>* meshVector = it->second;
		for (auto  i = meshVector->begin(); i != meshVector->end(); ++i)// Note: mesh could also be a skeletal mesh coponent
		{
			MeshComponent* mesh = *i;
			if (mesh->GetVisible())
			{
				mesh->Draw(mCurrentShader);
			}
		}
	}
}
bool Renderer::CreateMirrorTarget()
{
	// Generate a frame buffer for the mirror texture
	glGenFramebuffers(1, &mMirrorBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mMirrorBuffer);
	// Create the texture we'll use for rendering
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	mMirrorTexture = new Texture();
	mMirrorTexture->CreateForRendering(width, height, GL_RGB);
	// Add a depth buffer to this target
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	// Attach mirror texture as the output target for the frame buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mMirrorTexture->GetTextureID(), 0);
	// Set the list of buffers to draw to for this frame buffer
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	// Make sure everything worked
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// If it didn't work, delete the frame-buffer,
		// unload/delete the texture and return false
		glDeleteFramebuffers(1, &mMirrorBuffer);
		mMirrorTexture->Unload();
		delete mMirrorTexture;
		mMirrorTexture = nullptr;
		return false;
	}
	return true;
}
void Renderer::DrawFromGBuffer()
{
	// Clear the current frame-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Disable depth testing for the global lighting pass
	glDisable(GL_DEPTH_TEST);
	// Activate global G-buffer shader
	SetShader("Global");
	// Activate sprite vertices quad
	mSpriteVerts->SetActive();
	// Set the G-buffer textures to sample
	mGBuffer->SetTexturesActive();
	// Set the lighting uniforms
	SetLightUniforms(mCurrentShader, mView);
	// Draw the triangles
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	// Copy depth buffer from G-buffer to default frame buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mGBuffer->GetBufferID());
	int width = static_cast<int>(mScreenWidth);
	int height = static_cast<int>(mScreenHeight);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	// Enable depth test, but disable writes to depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	// Set the point light shader and mesh as active
	SetShader("PointLight");
	mPointLightMesh->GetVertexArray()->SetActive();
	// Set the view-projection matrix
	mCurrentShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// Set the G-buffer textures for sampling
	mGBuffer->SetTexturesActive();
	// The point light color should add to existing color
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	// Draw the point lights
	for (PointLightComponent* p : mPointLights)
	{
		p->Draw(mCurrentShader, mPointLightMesh);
	}
}
bool Renderer::LoadShaders()
{
	// Create sprite shader
	mCurrentShader = CreateShader("Sprite", "Shaders/Sprite");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	// Set the view-projection matrix
	matrix4 spriteViewProj = matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mCurrentShader->SetMatrixUniform("uViewProj", spriteViewProj);
	// Create basic mesh shader
	mCurrentShader = CreateShader("Mesh", "Shaders/Phong.vert", "Shaders/GBufferWrite.frag");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	// Set the view-projection matrix
	mView = matrix4::CreateLookAt(vector3::Zero, vector3::UnitZ, vector3::UnitY);
	mProjection = matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 10.0f, 10000.0f);
	mCurrentShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// Create skinned shader
	mCurrentShader = CreateShader("Skinned", "Shaders/Skinned.vert", "Shaders/GBufferWrite.frag");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	mCurrentShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// Create shader for drawing from GBuffer (global lighting)
	mCurrentShader = CreateShader("Global", "Shaders/GBufferGlobal");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	// For the GBuffer, we need to associate each sampler with an index
	mCurrentShader->SetIntUniform("uGTexColor", 0);
	mCurrentShader->SetIntUniform("uGNormal", 1);
	mCurrentShader->SetIntUniform("uGWorldPos", 2);
	// The view projection is just the sprite one
	mCurrentShader->SetMatrixUniform("uViewProj", spriteViewProj);
	// The world transform scales to the screen and flips y
	matrix4 gbufferWorld = matrix4::CreateScale(mScreenWidth, -mScreenHeight, 1.0f);
	mCurrentShader->SetMatrixUniform("uWorldTransform", gbufferWorld);
	// Create a shader for point lights from GBuffer
	mCurrentShader = CreateShader("PointLight","Shaders/BasicMesh.vert", "Shaders/GBufferPointLight.frag");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	// Set sampler indices
	mCurrentShader->SetIntUniform("uGTexColor", 0);
	mCurrentShader->SetIntUniform("uGNormal", 1);
	mCurrentShader->SetIntUniform("uGWorldPos", 2);
	mCurrentShader->SetVector2Uniform("uScreenDimensions", vector2(mScreenWidth, mScreenHeight));
	return true;
}
void Renderer::CreateSpriteVerts()
{
	float vertices[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top left
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top right
		0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom right
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom left
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTexRGB, indices, 6);
}
void Renderer::SetLightUniforms(Shader* shader, const matrix4& view)
{
	// Camera position is from inverted view
	matrix4 invView = view;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// Ambient light
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// Directional light
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecularColor", mDirLight.mSpecularColor);
}
vector3 Renderer::Unproject(const vector3& screenPoint) const
{
	// Convert screenPoint to device coordinates (between -1 and +1)
	vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (mScreenWidth) * 0.5f;
	deviceCoord.y /= (mScreenHeight) * 0.5f;
	// Transform vector by unprojection matrix
	matrix4 unprojection = mView * mProjection;
	unprojection.Invert();
	return vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}
void Renderer::GetScreenDirection(vector3& outStart, vector3& outDir) const
{
	// Get start point (in center of screen on near plane)
	vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = Unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	vector3 end = Unproject(screenPoint);
	// Get direction vector
	outDir = end - outStart;
	outDir.Normalize();
}