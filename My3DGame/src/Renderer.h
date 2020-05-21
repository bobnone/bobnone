#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Game.h"
#include "Math/Math.h"
#include "PointLightComponent.h"
#include "GBuffer.h"
#include "Texture.h"
#include "TextureHandler.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PointLightComponent.h"

struct DirectionalLight
{
	//Direction of light
	Vector3 direction_;
	//Diffuse color
	Vector3 diffuseColor_;
	//Specular color
	Vector3 specularColor_;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();
	bool initialize(float screenWidth, float screenHeight);
	void shutdown();
	void unloadData();
	void draw();
	void addSprite(class SpriteComponent* sprite);
	void removeSprite(class SpriteComponent* sprite);
	void addMeshComp(class MeshComponent* mesh);
	void removeMeshComp(class MeshComponent* mesh);
	void addPointLight(class PointLightComponent* light);
	void removePointLight(class PointLightComponent* light);
	class Texture* getTexture(const std::string& fileName);
	void removeTexture(const std::string& name);
	void removeTexture(class Texture* tex);
	class Mesh* getMesh(const std::string& fileName);
	void removeMesh(const std::string& name);
	void removeMesh(class Mesh* mesh);
	/*
		create and load a new shader using "fileName" for all shader files
		Note: sets the new shader as the active shader
	*/
	class Shader* createShader(const std::string& name, const std::string& fileName);
	/*
		create and load a new shader using the respective file names for the shader files
		Note: sets the new shader as the active shader
	*/
	class Shader* createShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile);
	// Returns the requested shader if it exist
	class Shader* getShader(const std::string& name);
	void removeShader(const std::string& name);
	void removeShader(class Shader* mesh);
	std::vector<class MeshComponent*>* linkMeshToShader(class MeshComponent* mesh, class Shader* shader);
	bool linkMeshesToShader(class Shader* shader, std::vector<class MeshComponent*>* meshVector);
	std::vector<class MeshComponent*>* getMeshShader(class Shader* shader);
	class Shader* getShaderFromMesh(class MeshComponent* mesh);
	void unlinkShader(class Shader* shader);
	void unlinkMesh(class MeshComponent* mesh);
	void unlinkMeshes(std::vector<class MeshComponent*>* meshVector);
	void setViewMatrix(const Matrix4x4& view)
	{
		view_ = view;
	}
	const Vector3& ambientLight() const
	{
		return ambientLight_;
	}
	void setAmbientLight(const Vector3& ambient)
	{
		ambientLight_ = ambient;
	}
	DirectionalLight& getDirectionalLight()
	{
		return dirLight_;
	}
	const std::vector<class PointLightComponent*>& pointLights() const
	{
		return pointLights_;
	}
	// Given a screen space point, unprojects it into world space,
	// based on the current 3D view/projection matrices
	// Expected ranges:
	// x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1) -- 0 is closer to camera, 1 is further
	Vector3 unproject(const Vector3& screenPoint) const;
	// Gets start point and direction of screen vector
	void getScreenDirection(Vector3& outStart, Vector3& outDir) const;
	float screenWidth() const
	{
		return screenWidth_;
	}
	float screenHeight() const
	{
		return screenHeight_;
	}
	void setMirrorView(const Matrix4x4& view)
	{
		mirrorView_ = view;
	}
	class Texture* mirrorTexture()
	{
		return mirrorTexture_;
	}
	class GBuffer* gBuffer()
	{
		return gBuffer_;
	}
private:
	void draw3DScene(unsigned int framebuffer, const Matrix4x4& view, const Matrix4x4& proj, bool lit = true);
	bool createMirrorTarget();
	void drawFromGBuffer();
	bool loadShaders();
	void createSpriteVerts();
	void setLightUniforms(class Shader* shader, const Matrix4x4& view);
	//Gets the requested shader and sets it as the active shader
	void setShader(const std::string& name);
	//Map of textures loaded
	std::unordered_map<std::string, class Texture*> textures_;
	//Map of meshes loaded
	std::unordered_map<std::string, class Mesh*> meshes_;
	//Map of shaders to mesh components
	std::unordered_map<class Shader*, std::vector<class MeshComponent*>*> meshShaders_;
	//Map of loaded shaders
	std::unordered_map<std::string, class Shader*> shaders_;
	//All the sprite components drawn
	std::vector<class SpriteComponent*> sprites_;
	//All (non-skeletal) mesh components drawn
	std::vector<class MeshComponent*> meshComps_;
	std::vector<class SkeletalMeshComponent*> skeletalMeshComps_;
	//Game
	class Game* game_;
	//TextureHandler
	class TextureHandler* textureHandler_;
	//The currently active shader
	class Shader* currentShader_;
	//Sprite vertex array
	class VertexArray* spriteVerts_;
	//View/projection for 3D shaders
	class Matrix4x4 view_;
	class Matrix4x4 projection_;
	//Lighting data
	class Vector3 ambientLight_;
	struct DirectionalLight dirLight_;
	//Window
	SDL_Window* window_;
	//OpenGL context
	SDL_GLContext context_;
	//Width/height
	float screenWidth_;
	float screenHeight_;
	unsigned int mirrorBuffer_;
	class Texture* mirrorTexture_;
	class Matrix4x4 mirrorView_;
	class GBuffer* gBuffer_;
	std::vector<class PointLightComponent*> pointLights_;
	class Mesh* pointLightMesh_;
	class Texture* defaultTexture_;
};