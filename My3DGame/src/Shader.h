//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#pragma once
#include <GL/glew.h>
#include <string>
#include "Math.h"

class Shader
{
public:
	Shader(std::string name);
	~Shader();
	bool load(const std::string& vertName, const std::string& fragName);
	void unload();
	//Set this as the active shader program
	void setActive();
	//Sets a Matrix uniform
	void setMatrixUniform(const char* name, const Matrix4& matrix);
	//Sets an array of matrix uniforms
	void setMatrixUniforms(const char* name, Matrix4* matrices, unsigned count);
	//Sets a Vector3 uniform
	void setVectorUniform(const char* name, const Vector3& vector);
	void setVector2Uniform(const char* name, const Vector2& vector);
	//Sets a float uniform
	void setFloatUniform(const char* name, float value);
	//Sets an integer uniform
	void setIntUniform(const char* name, int value);
	std::string vertexFile()
	{
		return vertexFile_;
	}
	std::string fragmentFile()
	{
		return fragmentFile_;
	}
	GLuint getVertexID()
	{
		return vertexShader_;
	}
	GLuint getFragmentID()
	{
		return fragmentShader_;
	}
	GLuint getID()
	{
		return shaderProgram_;
	}
	std::string name()
	{
		return name_;
	}
private:
	//Tries to compile the specified shader
	bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	//Tests whether shader compiled successfully
	bool isCompiled(GLuint shader);
	//Tests whether vertex/fragment programs link
	bool isValidProgram();
private:
	//Store the shader file paths
	std::string vertexFile_;
	std::string fragmentFile_;
	//Store the shader object IDs
	GLuint vertexShader_;
	GLuint fragmentShader_;
	GLuint shaderProgram_;
	//Human readable name
	std::string name_;
};