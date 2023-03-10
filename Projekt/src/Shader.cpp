#include "Shader.h"
#include "Debug.h"

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

#include <format>

Shader::Shader()
	: mProgramID(0), mShaderIDs{} 
{
}

Shader::~Shader()
{
	glDeleteProgram(mProgramID);
}

void Shader::Create()
{
	mProgramID = glCreateProgram();
}

void Shader::AddShader(const std::string& filepath, const int type)
{
	// get source
	const std::string shaderSourceString = GetShaderSource(filepath);
	const char* shaderSourceCString = shaderSourceString.c_str();

	// compile shader
	unsigned int index = GetIndex(type);
	mShaderIDs[index] = glCreateShader(type);
	unsigned int id = mShaderIDs[index];
	glShaderSource(id, 1, &shaderSourceCString, nullptr);
	glCompileShader(id);

	// check for compilation errors
	const std::string typeAsString = GetTypeAsString(type);
	CheckCompileErrors(id, typeAsString);

	// attach shader to program
	glAttachShader(mProgramID, id);
}

void Shader::LinkShader()
{
	glLinkProgram(mProgramID);
	CheckCompileErrors(mProgramID, "PROGRAM");

	glDeleteShader(mShaderIDs[VERTEX_SHADER]);
	glDeleteShader(mShaderIDs[FRAGMENT_SHADER]);
	for (int i = 2; i < MAX_AMOUNT_OF_SHADERS; ++i)
	{
		if (mShaderIDs[i] != 0)
		{
			glDeleteShader(mShaderIDs[i]);
		}
	}
}

void Shader::Bind() const
{
	glUseProgram(mProgramID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value);
}

void Shader::SetMat4(const std::string& name, const glm::mat4 &matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, false, &matrix[0][0]);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &vector) const
{
	glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &vector[0]);
}

std::string Shader::GetShaderSource(const std::string& filepath) const
{
	std::ifstream stream(filepath);
	std::stringstream buffer;
	buffer << stream.rdbuf();
	std::string shaderSourceString = buffer.str();
	return shaderSourceString;
}

unsigned int Shader::GetIndex(int type) const
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		return VERTEX_SHADER;
	case GL_FRAGMENT_SHADER:
		return FRAGMENT_SHADER;
	case GL_TESS_CONTROL_SHADER:
		return TESSELATION_CONTROL_SHADER;
	case GL_TESS_EVALUATION_SHADER:
		return TESSELATION_EVAL_SHADER;
	case GL_GEOMETRY_SHADER:
		return GEOMETRY_SHADER;
	case GL_COMPUTE_SHADER:
		return COMPUTE_SHADER;
	}
	return 6;
}

void Shader::CheckCompileErrors(unsigned int shader, const std::string& type)
{
	char message[1024];
	int success;

	if (type == "PROGRAM")
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, message);
			std::cout << "[Shader Program] Linking failed" << '\n';
			std::cout << message << '\n';
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, message);
			std::cout << std::format("[{} Shader] Compilation failed", type) 
				<< '\n';
			std::cout << message << '\n';
		}
	}
}

std::string Shader::GetTypeAsString(int type) const
{
	switch (type)
	{
	case GL_VERTEX_SHADER:
		return "Vertex";
	case GL_FRAGMENT_SHADER:
		return "Fragment";
	case GL_TESS_CONTROL_SHADER:
		return "Tesselation Control";
	case GL_TESS_EVALUATION_SHADER:
		return "Tesselation Evaluation";
	case GL_GEOMETRY_SHADER:
		return "Geometry";
	case GL_COMPUTE_SHADER:
		return "Compute";
	}
	ASSERT(0);
	return "Unreachable";
}
