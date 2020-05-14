//----------------------------------------------------------------
//From Game Programming in C++ by Sanjay Madhav
//Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//Released under the BSD License
//See LICENSE in root directory for full details.
//----------------------------------------------------------------

#include "Shader.h"
#include <SDL/SDL.h>
#include <fstream>
#include <sstream>

Shader::Shader(std::string name): shaderProgram_(0), vertexShader_(0), fragmentShader_(0)
{
	name_ = name;
}
Shader::~Shader()
{
	//EMPTY:
}
bool Shader::load(const std::string& vertFile, const std::string& fragFile)
{
	//Compile vertex and pixel shaders
	if(!compileShader(vertFile, GL_VERTEX_SHADER, vertexShader_) || !compileShader(fragFile, GL_FRAGMENT_SHADER, fragmentShader_))
	{
		return false;
	}
	vertexFile_ = vertFile;
	fragmentFile_ = fragFile;
	//Now create a shader program that
	//links together the vertex/fragment shaders
	shaderProgram_ = glCreateProgram();
	glAttachShader(shaderProgram_, vertexShader_);
	glAttachShader(shaderProgram_, fragmentShader_);
	glLinkProgram(shaderProgram_);
	//Verify that the program linked successfully
	if(!isValidProgram())
	{
		return false;
	}
	return true;
}
void Shader::unload()
{
	//Delete the program/shaders
	glDeleteProgram(shaderProgram_);
	glDeleteShader(vertexShader_);
	glDeleteShader(fragmentShader_);
}
void Shader::setActive()
{
	//Set this program as the active one
	glUseProgram(shaderProgram_);
}
void Shader::setMatrixUniform(const char* name, const Matrix4& matrix)
{
	//Find the uniform by this name
	GLuint loc = glGetUniformLocation(shaderProgram_, name);
	//Send the matrix data to the uniform
	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.getAsFloatPtr());
}
void Shader::setMatrixUniforms(const char* name, Matrix4* matrices, unsigned count)
{
	GLuint loc = glGetUniformLocation(shaderProgram_, name);
	//Send the matrix data to the uniform
	glUniformMatrix4fv(loc, count, GL_TRUE, matrices->getAsFloatPtr());
}
void Shader::setVectorUniform(const char* name, const Vector3& vector)
{
	GLuint loc = glGetUniformLocation(shaderProgram_, name);
	//Send the vector data
	glUniform3fv(loc, 1, vector.getAsFloatPtr());
}
void Shader::setVector2Uniform(const char* name, const Vector2& vector)
{
	GLuint loc = glGetUniformLocation(shaderProgram_, name);
	//Send the vector data
	glUniform2fv(loc, 1, vector.getAsFloatPtr());
}
void Shader::setFloatUniform(const char* name, float value)
{
	GLuint loc = glGetUniformLocation(shaderProgram_, name);
	//Send the float data
	glUniform1f(loc, value);
}
void Shader::setIntUniform(const char* name, int value)
{
	GLuint loc = glGetUniformLocation(shaderProgram_, name);
	//Send the float data
	glUniform1i(loc, value);
}
bool Shader::compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	//Open file
	std::ifstream shaderFile(fileName);
	if(shaderFile.is_open())
	{
		//Read all of the text into a string
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();
		//Create a shader of the specified type
		outShader = glCreateShader(shaderType);
		//Set the source characters and try to compile
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);
		if(!isCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());
			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());
		return false;
	}
	return true;
}
bool Shader::isCompiled(GLuint shader)
{
	GLint status;
	//Query the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(shader, 511, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);
		return false;
	}
	return true;
}
bool Shader::isValidProgram()
{
	GLint status;
	//Query the link status
	glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &status);
	if(status != GL_TRUE)
	{
		char buffer[512];
		memset(buffer, 0, 512);
		glGetProgramInfoLog(shaderProgram_, 511, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);
		return false;
	}
	return true;
}