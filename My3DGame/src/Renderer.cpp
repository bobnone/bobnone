// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Renderer.h"
#include "Texture.h"
#include "Mesh.h"
#include <algorithm>
#include "Shader.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "UIScreen.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
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
	//if (!CreateMirrorTarget())
	//{
	//	SDL_Log("Failed to create render target for mirror.");
	//	return false;
	//}
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
	// Destroy meshShaders
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
	//Draw3DScene(mMirrorBuffer, mMirrorView, mProjection);
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
	mCurrentShader = SetShader("Sprite");
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
		mSkeletalMeshes.emplace_back(sk);
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
		auto i = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(i);
	}
	else
	{
		auto i = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
		mMeshComps.erase(i);
	}
}
void Renderer::AddPointLight(PointLightComponent* light)
{
	mPointLights.emplace_back(light);
}
void Renderer::RemovePointLight(PointLightComponent* light)
{
	auto i = std::find(mPointLights.begin(), mPointLights.end(), light);
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
Shader* Renderer::NewShader(const std::string& name, const std::string& fileName)
{
	return NewShader(name, fileName, fileName);
}
Shader* Renderer::NewShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile)
{
	Shader* s = GetShader(name);
	if (s == nullptr)
	{
		s = new Shader();
		if (s->Load(vertexFile + ".vert", fragmentFile + ".frag"))
		{
			mShaders.emplace(name, s);
		}
		else
		{
			delete s;
			s = nullptr;
		}
	}
	return s;
}
Shader* Renderer::SetNewShader(const std::string& name, const std::string& fileName)
{
	Shader* s = NewShader(name, fileName);
	if (s != nullptr)
	{
		s->SetActive();
	}
	return s;
}
Shader* Renderer::SetNewShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile)
{
	Shader* s = NewShader(name, vertexFile, fragmentFile);
	if (s != nullptr)
	{
		s->SetActive();
	}
	return s;
}
Shader* Renderer::SetShader(const std::string& name)
{
	Shader* s = GetShader(name);
	if (s != nullptr)
	{
		s->SetActive();
	}
	return s;
}
Shader* Renderer::GetShader(const std::string& name)
{
	Shader* s = nullptr;
	auto i = mShaders.find(name);
	if (i != mShaders.end())
	{
		s = i->second;
	}
	return s;
}
void Renderer::Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, bool lit)
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
	// Set the mesh shader active
	mCurrentShader = SetShader("Mesh");
	// Update view-projection matrix
	mCurrentShader->SetMatrixUniform("uViewProj", view * proj);
	// Update lighting uniforms
	if (lit)
	{
		SetLightUniforms(mCurrentShader, view);
	}
	for (auto mc : mMeshComps)
	{
		if (mc->GetVisible())
		{
			mc->Draw(mCurrentShader);
		}
	}
	// Draw any skinned meshes now
	mCurrentShader = SetShader("Skinned");
	// Update view-projection matrix
	mCurrentShader->SetMatrixUniform("uViewProj", view * proj);
	// Update lighting uniforms
	if (lit)
	{
		SetLightUniforms(mCurrentShader, view);
	}
	for (auto sk : mSkeletalMeshes)
	{
		if (sk->GetVisible())
		{
			sk->Draw(mCurrentShader);
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
	mCurrentShader = SetShader("Global");
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
	mCurrentShader = SetShader("PointLight");
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
	mCurrentShader = SetNewShader("Sprite", "Shaders/Sprite");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	// Set the view-projection matrix
	Matrix4 spriteViewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mCurrentShader->SetMatrixUniform("uViewProj", spriteViewProj);
	// Create basic mesh shader
	mCurrentShader = SetNewShader("Mesh", "Shaders/Phong", "Shaders/GBufferWrite");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	// Set the view-projection matrix
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), mScreenWidth, mScreenHeight, 10.0f, 10000.0f);
	mCurrentShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// Create skinned shader
	mCurrentShader = SetNewShader("Skinned", "Shaders/Skinned", "Shaders/GBufferWrite");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	mCurrentShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// Create shader for drawing from GBuffer (global lighting)
	mCurrentShader = SetNewShader("Global", "Shaders/GBufferGlobal");
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
	Matrix4 gbufferWorld = Matrix4::CreateScale(mScreenWidth, -mScreenHeight, 1.0f);
	mCurrentShader->SetMatrixUniform("uWorldTransform", gbufferWorld);
	// Create a shader for point lights from GBuffer
	mCurrentShader = SetNewShader("PointLight", "Shaders/BasicMesh", "Shaders/GBufferPointLight");
	if (mCurrentShader == nullptr)
	{
		return false;
	}
	// Set sampler indices
	mCurrentShader->SetIntUniform("uGTexColor", 0);
	mCurrentShader->SetIntUniform("uGNormal", 1);
	mCurrentShader->SetIntUniform("uGWorldPos", 2);
	mCurrentShader->SetVector2Uniform("uScreenDimensions", Vector2(mScreenWidth, mScreenHeight));
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
void Renderer::SetLightUniforms(Shader* shader, const Matrix4& view)
{
	// Camera position is from inverted view
	Matrix4 invView = view;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
	// Ambient light
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);
	// Directional light
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecularColor", mDirLight.mSpecularColor);
}
Vector3 Renderer::Unproject(const Vector3& screenPoint) const
{
	// Convert screenPoint to device coordinates (between -1 and +1)
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (mScreenWidth) * 0.5f;
	deviceCoord.y /= (mScreenHeight) * 0.5f;
	// Transform vector by unprojection matrix
	Matrix4 unprojection = mView * mProjection;
	unprojection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}
void Renderer::GetScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = Unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = Unproject(screenPoint);
	// Get direction vector
	outDir = end - outStart;
	outDir.Normalize();
}