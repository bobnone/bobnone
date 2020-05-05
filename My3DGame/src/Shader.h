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
#include "Math.h"

class Shader
{
public:
	Shader(std::string name);
	~Shader();
	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();
	// Set this as the active shader program
	void SetActive();
	// Sets a Matrix uniform
	void SetMatrixUniform(const char* name, const matrix4& matrix);
	// Sets an array of matrix uniforms
	void SetMatrixUniforms(const char* name, matrix4* matrices, unsigned count);
	// Sets a Vector3 uniform
	void SetVectorUniform(const char* name, const vector3& vector);
	void SetVector2Uniform(const char* name, const vector2& vector);
	// Sets a float uniform
	void SetFloatUniform(const char* name, float value);
	// Sets an integer uniform
	void SetIntUniform(const char* name, int value);
	std::string GetVertexFile()
	{
		return mVertexFile;
	}
	std::string GetFragmentFile()
	{
		return mFragmentFile;
	}
	GLuint GetVertexID()
	{
		return mVertexShader;
	}
	GLuint GetFragmentID()
	{
		return mFragmentShader;
	}
	GLuint GetID()
	{
		return mShaderProgram;
	}
	std::string GetName()
	{
		return mName;
	}
private:
	// Tries to compile the specified shader
	bool CompileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	// Tests whether shader compiled successfully
	bool IsCompiled(GLuint shader);
	// Tests whether vertex/fragment programs link
	bool IsValidProgram();
private:
	// Store the shader file paths
	std::string mVertexFile;
	std::string mFragmentFile;
	// Store the shader object IDs
	GLuint mVertexShader;
	GLuint mFragmentShader;
	GLuint mShaderProgram;
	// Human readable name
	std::string mName;
};