#include "Renderer.h"
#include <iostream>
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include "SkeletalMeshComponent.h"
#include "UIScreen.h"
#include "GBuffer.h"

Renderer::Renderer(Game* game):game_(game), mirrorBuffer_(0)
{
	//EMPTY:
}
Renderer::~Renderer()
{
	//EMPTY:
}
bool Renderer::initialize(float screenWidth, float screenHeight)
{
	screenWidth_ = screenWidth;
	screenHeight_ = screenHeight;
	//Set OpenGL attributes
	//Use the core OpenGL profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//Specify version 3.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Force OpenGL to use hardware acceleration
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	window_ = SDL_CreateWindow("<Window Title>", 100, 100, static_cast<int>(screenWidth_), static_cast<int>(screenHeight_), SDL_WINDOW_OPENGL);
	if(!window_)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	//Create an OpenGL context
	context_ = SDL_GL_CreateContext(window_);
	//Initialize GLEW
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}
	//On some platforms, GLEW will emit a benign error code,
	//so clear it
	glGetError();
	//Make sure we can create/compile shaders
	if(!loadShaders())
	{
		SDL_Log("Failed to load shaders.");
		return false;
	}
	//Create quad for drawing sprites
	createSpriteVerts();
	//Create render target for mirror
	if(!createMirrorTarget())
	{
		SDL_Log("Failed to create render target for mirror.");
		return false;
	}
	//Create TextureHandler
	textureHandler_ = new TextureHandler();
	if(!textureHandler_->initialize())
	{
		SDL_Log("Failed to create TextureHandler.");
		return false;
	}
	defaultTexture_ = new Texture("Assets/Default.png");
	if(!defaultTexture_->isLoaded())
	{
		SDL_Log("Failed to create a default texture.");
		exit(1);
	}
	//Create G-buffer
	gBuffer_ = new GBuffer();
	int width = static_cast<int>(screenWidth_);
	int height = static_cast<int>(screenHeight_);
	if(!gBuffer_->create(width, height))
	{
		SDL_Log("Failed to create G-buffer.");
		return false;
	}
	//Load point light mesh
	pointLightMesh_ = getMesh("Assets/PointLight.gpmesh");
	return true;
}
void Renderer::shutdown()
{
	//Get rid of any render target textures, if they exist
	if(mirrorTexture_ != nullptr)
	{
		glDeleteFramebuffers(1, &mirrorBuffer_);
		mirrorTexture_->~Texture();
		delete mirrorTexture_;
	}
	//Get rid of G-buffer
	if(gBuffer_ != nullptr)
	{
		gBuffer_->destroy();
		delete gBuffer_;
	}
	//Delete point lights
	while(!pointLights_.empty())
	{
		delete pointLights_.back();
	}
	delete spriteVerts_;
	//Unset mCurrentShader pointer so you don't delete any actual shaders
	currentShader_ = nullptr;
	delete currentShader_;
	delete textureHandler_;
	textureHandler_ = nullptr;
	SDL_GL_DeleteContext(context_);
	SDL_DestroyWindow(window_);
}
void Renderer::unloadData()
{
	//Destroy meshShaders
	//Note: shaders and MeshComponents get deleted somewhere else
	meshShaders_.clear();
	//Destroy textures
	for(auto i : textures_)
	{
		i.second->~Texture();
		delete i.second;
	}
	textures_.clear();
	//Destroy meshes
	for(auto i: meshes_)
	{
		i.second->unload();
		delete i.second;
	}
	meshes_.clear();
	//Destroy shaders
	for(auto i: shaders_)
	{
		i.second->unload();
		delete i.second;
	}
	shaders_.clear();
}
void Renderer::draw()
{
	//Draw to the mirror texture first
	draw3DScene(mirrorBuffer_, mirrorView_, projection_);
	//Draw the 3D scene to the G-buffer
	draw3DScene(gBuffer_->bufferID(), view_, projection_, false);
	//Set the frame buffer back to zero (screen's frame buffer)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Draw from the GBuffer
	drawFromGBuffer();
	//Draw all sprite components
	//Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	//Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	//Set shader/VAO as active
	setShader("Sprite");
	spriteVerts_->setActive();
	for(auto sprite: sprites_)
	{
		if(sprite->visible())
		{
			sprite->draw(currentShader_);
		}
	}
	//Draw any UI screens
	for(auto ui : game_->UIStack())
	{
		ui->draw(currentShader_);
	}
	//Swap the buffers
	SDL_GL_SwapWindow(window_);
}
void Renderer::addSprite(SpriteComponent* sprite)
{
	//Find the insertion point in the sorted vector
	//(The first element with a higher draw order than me)
	int myDrawOrder = sprite->drawOrder();
	auto i = sprites_.begin();
	for(; i != sprites_.end(); ++i)
	{
		if(myDrawOrder < (*i)->drawOrder())
		{
			break;
		}
	}
	//Inserts element before position of iterator
	sprites_.insert(i, sprite);
}
void Renderer::removeSprite(SpriteComponent* sprite)
{
	auto i = std::find(sprites_.begin(), sprites_.end(), sprite);
	sprites_.erase(i);
}
void Renderer::addMeshComp(MeshComponent* mesh)
{
	if (mesh->isSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		skeletalMeshComps_.emplace_back(sk);
	}
	else
	{
		meshComps_.emplace_back(mesh);
	}
}
void Renderer::removeMeshComp(MeshComponent* mesh)
{
	if(mesh->isSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto i = std::find(skeletalMeshComps_.begin(), skeletalMeshComps_.end(), sk);
		skeletalMeshComps_.erase(i);
	}
	else
	{
		auto i = std::find(meshComps_.begin(), meshComps_.end(), mesh);
		meshComps_.erase(i);
	}
	unlinkMesh(mesh);
}
void Renderer::addPointLight(PointLightComponent* light)
{
	pointLights_.emplace_back(light);
}
void Renderer::removePointLight(PointLightComponent* light)
{
	auto i = find(pointLights_.begin(), pointLights_.end(), light);
	pointLights_.erase(i);
}
Texture* Renderer::getTexture(const std::string& fileName)
{
	Texture* tex = nullptr;
	auto i = textures_.find(fileName);
	if(i != textures_.end())
	{
		tex = i->second;
	}
	else
	{
		tex = new Texture(fileName);
		if(tex->isLoaded())
		{
			textures_.emplace(fileName, tex);
		}
		else
		{
			return defaultTexture_;
		}
	}
	return tex;
}
void Renderer::removeTexture(const std::string& name)
{
	auto it = textures_.find(name);
	if (it != textures_.end())
	{
		textures_.erase(it);
	}
}
void Renderer::removeTexture(Texture* tex)
{
	auto it = textures_.begin();
	while(it != textures_.end())
	{
		if(it->second == tex)
		{
			break;
		}
		it++;
	}
	if(it != textures_.end())
	{
		textures_.erase(it);
	}
}
Mesh* Renderer::getMesh(const std::string & fileName)
{
	Mesh* m = nullptr;
	auto i = meshes_.find(fileName);
	if (i != meshes_.end())
	{
		m = i->second;
	}
	else
	{
		m = new Mesh();
		if (m->load(fileName, this))
		{
			meshes_.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}
void Renderer::removeMesh(const std::string& name)
{
	auto it = meshes_.find(name);
	if (it != meshes_.end())
	{
		meshes_.erase(it);
	}
}
void Renderer::removeMesh(Mesh* mesh)
{
	auto it = meshes_.begin();
	while(it != meshes_.end())
	{
		if(it->second == mesh)
		{
			break;
		}
		it++;
	}
	if(it != meshes_.end())
	{
		meshes_.erase(it);
	}
}
Shader* Renderer::createShader(const std::string& name, const std::string& fileName)
{
	return createShader(name, fileName+".vert", fileName+".frag");
}
Shader* Renderer::createShader(const std::string& name, const std::string & vertexFile, const std::string & fragmentFile)
{
	Shader* s = new Shader(name);
	if (s->load(vertexFile, fragmentFile))
	{
		shaders_.emplace(s->name(), s);
		s->setActive();
	}
	else
	{
		delete s;
		s = nullptr;
	}
	return s;
}
Shader* Renderer::getShader(const std::string& name)
{
	auto i = shaders_.find(name);
	if(i != shaders_.end())
	{
		return i->second;
	}
	else
	{
		std::cerr << "Failed to get shader \"" << name << "\"" << std::endl;
		return nullptr;
	}
}
void Renderer::setShader(const std::string& name)
{
	Shader* s = getShader(name);
	currentShader_ = s;
	s->setActive();
}
void Renderer::removeShader(const std::string& name)
{
	auto it = shaders_.find(name);
	if(it != shaders_.end())
	{
		shaders_.erase(it);
	}
}
void Renderer::removeShader(Shader* shader)
{
	auto it = shaders_.begin();
	while(it != shaders_.end())
	{
		if(it->second == shader)
		{
			break;
		}
		it++;
	}
	if(it != shaders_.end())
	{
		shaders_.erase(it);
	}
}
std::vector<MeshComponent*>* Renderer::linkMeshToShader(MeshComponent* mesh, Shader* shader)
{
	std::vector<MeshComponent*>* meshVector = getMeshShader(shader);
	if (meshVector == nullptr)
	{
		meshVector = new std::vector<MeshComponent*>();
		meshVector->emplace_back(mesh);
		meshShaders_.emplace(shader, meshVector);
	}
	else
	{
		meshVector->emplace_back(mesh);
	}
	return meshVector;
}
bool Renderer::linkMeshesToShader(Shader* shader, std::vector<MeshComponent*>* meshVector)
{
	if(getMeshShader(shader) != nullptr)
	{
		return false;
	}
	meshShaders_.emplace(shader, meshVector);
	return true;
}
std::vector<MeshComponent*>* Renderer::getMeshShader(Shader* shader)
{
	std::vector<MeshComponent*>* meshVector = nullptr;
	auto it = meshShaders_.find(shader);
	if(it != meshShaders_.end())
	{
		meshVector = it->second;
	}
	return meshVector;
}
Shader* Renderer::getShaderFromMesh(MeshComponent* mesh)
{
	auto it = meshShaders_.begin();
	while (it != meshShaders_.end())
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
void Renderer::unlinkShader(Shader* shader)
{
	auto it = meshShaders_.find(shader);
	if (it != meshShaders_.end())
	{
		meshShaders_.erase(it);
	}
}
void Renderer::unlinkMesh(MeshComponent* mesh)
{
	auto it = meshShaders_.begin();
	while (it != meshShaders_.end())
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
void Renderer::unlinkMeshes(std::vector<MeshComponent*>* meshVector)
{
	auto it = meshShaders_.begin();
	while(it != meshShaders_.end())
	{
		if(it->second == meshVector)
		{
			break;
		}
		it++;
	}
	if(it != meshShaders_.end())
	{
		meshShaders_.erase(it);
	}
}
void Renderer::draw3DScene(unsigned int framebuffer, const Matrix4x4& view, const Matrix4x4& proj, bool lit)
{
	//Set the current frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//Clear color buffer/depth buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Draw mesh components
	//Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	for (auto it = meshShaders_.begin(); it != meshShaders_.end(); it++)
	{
		//Set the specific mesh shader as active
		currentShader_ = it->first;
		currentShader_->setActive();
		//Update view-projection matrix
		currentShader_->setMatrixUniform("uViewProj", view * proj);
		//Update lighting uniforms
		if(lit)
		{
			setLightUniforms(currentShader_, view);
		}
		std::vector<MeshComponent*>* meshVector = it->second;
		//Note: mesh could also be a skeletal mesh component
		for(auto  i = meshVector->begin(); i != meshVector->end(); ++i)
		{
			MeshComponent* mesh = *i;
			if(mesh->visible())
			{
				mesh->draw(currentShader_);
			}
		}
	}
}
bool Renderer::createMirrorTarget()
{
	//Generate a frame buffer for the mirror texture
	glGenFramebuffers(1, &mirrorBuffer_);
	glBindFramebuffer(GL_FRAMEBUFFER, mirrorBuffer_);
	//Create the texture we'll use for rendering
	int width = static_cast<int>(screenWidth_);
	int height = static_cast<int>(screenHeight_);
	mirrorTexture_ = new Texture(width, height, GL_RGB);
	//Add a depth buffer to this target
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	//Attach mirror texture as the output target for the frame buffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mirrorTexture_->textureID(), 0);
	//Set the list of buffers to draw to for this frame buffer
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	//Make sure everything worked
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		//If it didn't work, delete the frame-buffer,
		//unload/delete the texture and return false
		glDeleteFramebuffers(1, &mirrorBuffer_);
		mirrorTexture_->~Texture();
		delete mirrorTexture_;
		mirrorTexture_ = nullptr;
		return false;
	}
	return true;
}
void Renderer::drawFromGBuffer()
{
	//Clear the current frame-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Disable depth testing for the global lighting pass
	glDisable(GL_DEPTH_TEST);
	//Activate global G-buffer shader
	setShader("Global");
	//Activate sprite vertices quad
	spriteVerts_->setActive();
	//Set the G-buffer textures to sample
	gBuffer_->setTexturesActive();
	//Set the lighting uniforms
	setLightUniforms(currentShader_, view_);
	//Draw the triangles
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	//Copy depth buffer from G-buffer to default frame buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer_->bufferID());
	int width = static_cast<int>(screenWidth_);
	int height = static_cast<int>(screenHeight_);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	//Enable depth test, but disable writes to depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	//Set the point light shader and mesh as active
	setShader("PointLight");
	pointLightMesh_->vertexArray()->setActive();
	//Set the view-projection matrix
	currentShader_->setMatrixUniform("uViewProj", view_ * projection_);
	//Set the G-buffer textures for sampling
	gBuffer_->setTexturesActive();
	//The point light color should add to existing color
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	//Draw the point lights
	for(PointLightComponent* p: pointLights_)
	{
		p->draw(currentShader_, pointLightMesh_);
	}
}
bool Renderer::loadShaders()
{
	//Create sprite shader
	currentShader_ = createShader("Sprite", "Shaders/Sprite");
	if(currentShader_ == nullptr)
	{
		return false;
	}
	//Set the view-projection matrix
	Matrix4x4 spriteViewProj = Math::createSimpleViewProjectionMatrix(screenWidth_, screenHeight_);
	currentShader_->setMatrixUniform("uViewProj", spriteViewProj);
	//Create basic mesh shader
	currentShader_ = createShader("Mesh", "Shaders/Phong.vert", "Shaders/GBufferWrite.frag");
	if(currentShader_ == nullptr)
	{
		return false;
	}
	//Set the view-projection matrix
	view_ = Math::lookAt(Vector3(), Vector3::UNIT_Z, Vector3::UNIT_Y);
	projection_ = Math::perspectiveFOV(Math::toRadians(70.0f), screenWidth_, screenHeight_, 10.0f, 10000.0f);
	currentShader_->setMatrixUniform("uViewProj", view_ * projection_);
	//Create skinned shader
	currentShader_ = createShader("Skinned", "Shaders/Skinned.vert", "Shaders/GBufferWrite.frag");
	if(currentShader_ == nullptr)
	{
		return false;
	}
	currentShader_->setMatrixUniform("uViewProj", view_ * projection_);
	//Create shader for drawing from GBuffer (global lighting)
	currentShader_ = createShader("Global", "Shaders/GBufferGlobal");
	if(currentShader_ == nullptr)
	{
		return false;
	}
	//For the GBuffer, we need to associate each sampler with an index
	currentShader_->setIntUniform("uGTexColor", 0);
	currentShader_->setIntUniform("uGNormal", 1);
	currentShader_->setIntUniform("uGWorldPos", 2);
	//The view projection is just the sprite one
	currentShader_->setMatrixUniform("uViewProj", spriteViewProj);
	//The world transform scales to the screen and flips y
	Matrix4x4 gbufferWorld = Math::createScaleMatrix(screenWidth_, -screenHeight_, 1.0f);
	currentShader_->setMatrixUniform("uWorldTransform", gbufferWorld);
	//Create a shader for point lights from GBuffer
	currentShader_ = createShader("PointLight","Shaders/BasicMesh.vert", "Shaders/GBufferPointLight.frag");
	if(currentShader_ == nullptr)
	{
		return false;
	}
	//Set sampler indices
	currentShader_->setIntUniform("uGTexColor", 0);
	currentShader_->setIntUniform("uGNormal", 1);
	currentShader_->setIntUniform("uGWorldPos", 2);
	currentShader_->setVector2Uniform("uScreenDimensions", Vector2(screenWidth_, screenHeight_));
	return true;
}
void Renderer::createSpriteVerts()
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
	spriteVerts_ = new VertexArray(vertices, 4, VertexArray::PosNormTexRGB, indices, 6);
}
void Renderer::setLightUniforms(Shader* shader, const Matrix4x4& view)
{
	//Camera position is from inverted view
	Matrix4x4 invView = view;
	invView.invert();
	shader->setVectorUniform("uCameraPos", invView.getTranslation());
	//Ambient light
	shader->setVectorUniform("uAmbientLight", ambientLight_);
	//Directional light
	shader->setVectorUniform("uDirLight.direction_", dirLight_.direction_);
	shader->setVectorUniform("uDirLight.diffuseColor_", dirLight_.diffuseColor_);
	shader->setVectorUniform("uDirLight.specularColor_", dirLight_.specularColor_);
}
Vector3 Renderer::unproject(const Vector3& screenPoint) const
{
	//Convert screenPoint to device coordinates (between -1 and +1)
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (screenWidth_) * 0.5f;
	deviceCoord.y /= (screenHeight_) * 0.5f;
	//Transform vector by unprojection matrix
	Matrix4x4 unprojection = view_ * projection_;
	unprojection.invert();
	return Math::transformWithPerspDiv(deviceCoord, unprojection);
}
void Renderer::getScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	//Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = unproject(screenPoint);
	//Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = unproject(screenPoint);
	//Get direction vector
	outDir = end - outStart;
	outDir.normalize();
}