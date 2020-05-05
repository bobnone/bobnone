// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Math.h"

using namespace std;

struct DirectionalLight
{
	// Direction of light
	Vector3 mDirection;
	// Diffuse color
	Vector3 mDiffuseColor;
	// Specular color
	Vector3 mSpecularColor;
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
	class Texture* GetTexture(const string& fileName);
	void RemoveTexture(const string& name);
	void RemoveTexture(class Texture* tex);
	class Mesh* GetMesh(const string& fileName);
	void RemoveMesh(const string& name);
	void RemoveMesh(class Mesh* mesh);
	/*
		create and load a new shader using "fileName" for all shader files
		Note: sets the new shader as the active shader
	*/
	class Shader* CreateShader(const string& name, const string& fileName);
	/*
		create and load a new shader using the respective file names for the shader files
		Note: sets the new shader as the active shader
	*/
	class Shader* CreateShader(const string& name, const string& vertexFile, const string& fragmentFile);
	// Returns the requested shader if it exist
	class Shader* GetShader(const string& name);
	void RemoveShader(const string& name);
	void RemoveShader(class Shader* mesh);
	vector<class MeshComponent*>* LinkMeshToShader(class MeshComponent* mesh, class Shader* shader);
	bool LinkMeshesToShader(class Shader* shader, vector<class MeshComponent*>* meshVector);
	vector<class MeshComponent*>* GetMeshShader(class Shader* shader);
	class Shader* GetShaderFromMesh(class MeshComponent* mesh);
	void UnlinkShader(class Shader* shader);
	void UnlinkMesh(class MeshComponent* mesh);
	void UnlinkMeshes(vector<class MeshComponent*>* meshVector);
	void SetViewMatrix(const Matrix4& view)
	{
		mView = view;
	}
	const Vector3& GetAmbientLight() const
	{
		return mAmbientLight;
	}
	void SetAmbientLight(const Vector3& ambient)
	{
		mAmbientLight = ambient;
	}
	DirectionalLight& GetDirectionalLight()
	{
		return mDirLight;
	}
	const vector<class PointLightComponent*>& GetPointLights() const
	{
		return mPointLights;
	}
	// Given a screen space point, unprojects it into world space,
	// based on the current 3D view/projection matrices
	// Expected ranges:
	// x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1) -- 0 is closer to camera, 1 is further
	Vector3 Unproject(const Vector3& screenPoint) const;
	// Gets start point and direction of screen vector
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;
	float GetScreenWidth() const
	{
		return mScreenWidth;
	}
	float GetScreenHeight() const
	{
		return mScreenHeight;
	}
	void SetMirrorView(const Matrix4& view)
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
	void Draw3DScene(unsigned int framebuffer, const Matrix4& view, const Matrix4& proj, bool lit = true);
	bool CreateMirrorTarget();
	void DrawFromGBuffer();
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader, const Matrix4& view);
	// Gets the requested shader and sets it as the active shader
	void SetShader(const string& name);
	// Map of textures loaded
	unordered_map<string, class Texture*> mTextures;
	// Map of meshes loaded
	unordered_map<string, class Mesh*> mMeshes;
	// Map of shaders to mesh components
	unordered_map<class Shader*, vector<class MeshComponent*>*> mMeshShaders;
	// Map of loaded shaders
	unordered_map<string, class Shader*> mShaders;
	// All the sprite components drawn
	vector<class SpriteComponent*> mSprites;
	// All (non-skeletal) mesh components drawn
	vector<class MeshComponent*> mMeshComps;
	vector<class SkeletalMeshComponent*> mSkeletalMeshComps;
	// Game
	class Game* mGame;
	// The currently active shader
	class Shader* mCurrentShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;
	// View/projection for 3D shaders
	Matrix4 mView;
	Matrix4 mProjection;
	// Lighting data
	Vector3 mAmbientLight;
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
	Matrix4 mMirrorView;
	class GBuffer* mGBuffer;
	vector<class PointLightComponent*> mPointLights;
	class Mesh* mPointLightMesh;
};