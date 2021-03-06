#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include "Input.h"

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	unsigned int ID;
	void UseShader();
	void SetFloat(const string &valueName, float value) const;
	void SetBool(const string &valueName, bool value) const;
	void SetInt(const string &valueName, int value) const;
	void SetVec4(const string &valueName, glm::vec4 value) const;
	void SetVec3(const string &valueName, glm::vec3 value) const;
	void SetVec2(const string &valueName, glm::vec2 value) const;
	void SetMatrix(const string &name, glm::mat4 value) const;
	
private:
	string ReadFile(const char* path);
	unsigned int CompileShader(const char* code, GLenum type);
	void LinkShader(unsigned int vertex, unsigned int fragment);
};
