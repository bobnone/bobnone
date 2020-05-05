#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Game.h"
#include "Math.h"
#include "PointLightComponent.h"
#include "GBuffer.h"
#include "Texture.h"
#include "TextureHandler.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PointLightComponent.h"

struct DirectionalLight
{
	// Direction of light
	vector3 mDirection;
	// Diffuse color
	vector3 mDiffuseColor;
	// Specular color
	vector3 mSpecularColor;
};
class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();
	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();
	void UnloadData();
	void Draw();
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);
	void AddPointLight(class PointLightComponent* light);
	void RemovePointLight(class PointLightComponent* light);
	class Texture* GetTexture(const std::string& fileName);
	void RemoveTexture(const std::string& name);
	void RemoveTexture(class Texture* tex);
	class Mesh* GetMesh(const std::string& fileName);
	void RemoveMesh(const std::string& name);
	void RemoveMesh(class Mesh* mesh);
	/*
		create and load a new shader using "fileName" for all shader files
		Note: sets the new shader as the active shader
	*/
	class Shader* CreateShader(const std::string& name, const std::string& fileName);
	/*
		create and load a new shader using the respective file names for the shader files
		Note: sets the new shader as the active shader
	*/
	class Shader* CreateShader(const std::string& name, const std::string& vertexFile, const std::string& fragmentFile);
	// Returns the requested shader if it exist
	class Shader* GetShader(const std::string& name);
	void RemoveShader(const std::string& name);
	void RemoveShader(class Shader* mesh);
	std::vector<class MeshComponent*>* LinkMeshToShader(class MeshComponent* mesh, class Shader* shader);
	bool LinkMeshesToShader(class Shader* shader, std::vector<class MeshComponent*>* meshVector);
	std::vector<class MeshComponent*>* GetMeshShader(class Shader* shader);
	class Shader* GetShaderFromMesh(class MeshComponent* mesh);
	void UnlinkShader(class Shader* shader);
	void UnlinkMesh(class MeshComponent* mesh);
	void UnlinkMeshes(std::vector<class MeshComponent*>* meshVector);
	void SetViewMatrix(const matrix4& view)
	{
		mView = view;
	}
	const vector3& GetAmbientLight() const
	{
		return mAmbientLight;
	}
	void SetAmbientLight(const vector3& ambient)
	{
		mAmbientLight = ambient;
	}
	DirectionalLight& GetDirectionalLight()
	{
		return mDirLight;
	}
	const std::vector<class PointLightComponent*>& GetPointLights() const
	{
		return mPointLights;
	}
	// Given a screen space point, unprojects it into world space,
	// based on the current 3D view/projection matrices
	// Expected ranges:
	// x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1) -- 0 is closer to camera, 1 is further
	vector3 Unproject(const vector3& screenPoint) const;
	// Gets start point and direction of screen vector
	void GetScreenDirection(vector3& outStart, vector3& outDir) const;
	float GetScreenWidth() const
	{
		return mScreenWidth;
	}
	float GetScreenHeight() const
	{
		return mScreenHeight;
	}
	void SetMirrorView(const matrix4& view)
	{
		mMirrorView = view;
	}
	class Texture* GetMirrorTexture()
	{
		return mMirrorTexture;
	}
	class GBuffer* GetGBuffer()
	{
		return mGBuffer;
	}
private:
	void Draw3DScene(unsigned int framebuffer, const matrix4& view, const matrix4& proj, bool lit = true);
	bool CreateMirrorTarget();
	void DrawFromGBuffer();
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader, const matrix4& view);
	// Gets the requested shader and sets it as the active shader
	void SetShader(const std::string& name);
	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;
	// Map of meshes loaded
	std::unordered_map<std::string, class Mesh*> mMeshes;
	// Map of shaders to mesh components
	std::unordered_map<class Shader*, std::vector<class MeshComponent*>*> mMeshShaders;
	// Map of loaded shaders
	std::unordered_map<std::string, class Shader*> mShaders;
	// All the sprite components drawn
	std::vector<class SpriteComponent*> mSprites;
	// All (non-skeletal) mesh components drawn
	std::vector<class MeshComponent*> mMeshComps;
	std::vector<class SkeletalMeshComponent*> mSkeletalMeshComps;
	// Game
	class Game* mGame;
	// TextureHandler
	class TextureHandler* mTextureHandler;
	// The currently active shader
	class Shader* mCurrentShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;
	// View/projection for 3D shaders
	matrix4 mView;
	matrix4 mProjection;
	// Lighting data
	vector3 mAmbientLight;
	DirectionalLight mDirLight;
	// Window
	SDL_Window* mWindow;
	// OpenGL context
	SDL_GLContext mContext;
	// Width/height
	float mScreenWidth;
	float mScreenHeight;
	unsigned int mMirrorBuffer;
	class Texture* mMirrorTexture;
	matrix4 mMirrorView;
	class GBuffer* mGBuffer;
	std::vector<class PointLightComponent*> mPointLights;
	class Mesh* mPointLightMesh;
};