#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Material.h"
#include "Shader.h"
#include "Texture.h"

constexpr unsigned int MAX_BONE_INFLUENCE = 4;

class Vertex
{
public:
	Vertex()
		: m_position(glm::vec3(0.0f))
		, m_normal(glm::vec3(0.0f))
		, m_texCoords(glm::vec3(0.0f))
	{}

	glm::vec3 m_position;
	glm::vec2 m_texCoords;
	glm::vec3 m_normal;

	//bone indexes which will influence this vertex
	int m_boneIDs[MAX_BONE_INFLUENCE] = { 0 };

	//weights from each bone
	float m_weights[MAX_BONE_INFLUENCE] = { 0.0f };
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();
	
	void Draw(Shader& shader, std::unordered_map<std::string, Material*>& materials); // I would really like the ability to assign shaders to objects, keep things together. But accessing an existing shader is better. Override maybe?
	void SetMaterialName(std::string materialName);

private:
	void SetupMesh();

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::string m_materialName;
};

