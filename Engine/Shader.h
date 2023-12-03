#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	// use/activate the shader
	void Use();
	
	// utility uniform functions
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void SetVec4(const std::string& name, glm::vec4 value) const;
	void SetMat4(const std::string& name, glm::mat4 value) const;

	// getter and setters
	int GetID();
	void SetID(int newID);

private:
	// the program ID
	unsigned int m_ID;
};