#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <string>
#include <vector>

#include "Shader.h"

constexpr unsigned int MAX_BONE_INFLUENCE = 4;

// TODO: Make these actual classes with functions and constructors?? Encapsulate it all in a namespace for now so that 
//		 I only need to be in one file but I can get some good OOP action going

class Vertex
{
public:
	Vertex()
		: m_position(glm::vec3(0.0f))
		, m_normal(glm::vec3(0.0f))
		, m_texCoords(glm::vec3(0.0f))
		, m_tangent(glm::vec3(0.0f))
		, m_biTangent(glm::vec3(0.0f))
	{}

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();

	void SetNormal(glm::vec3 normal);
	glm::vec3 GetNormal();

	void SetTexCoords(glm::vec2 texCoords);
	glm::vec2 GetTexCoords();

	void SetTangent(glm::vec3 tangent);
	glm::vec3 GetTangent();

	void SetBiTangent(glm::vec3 biTangent);
	glm::vec3 GetBiTangent();

private:
	glm::vec3 m_position;
	glm::vec2 m_texCoords;
	glm::vec3 m_normal;
	glm::vec3 m_tangent;
	glm::vec3 m_biTangent;

	//bone indexes which will influence this vertex
	int m_boneIDs[MAX_BONE_INFLUENCE] = { 0 };

	//weights from each bone
	float m_weights[MAX_BONE_INFLUENCE] = { 0.0f };
};

enum TextureType
{
	NONE = -1,
	DIFFUSE,
	NORMAL,
	ROUGHNESS,
	SPECULAR,
	AMBIENTOCCLUSION
};

struct Texture
{
public:
	Texture()
		: m_id(0)
		, m_type(TextureType::NONE)
		, m_path("")
	{}

	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

private:
	unsigned int m_id;
	TextureType m_type;
	std::filesystem::path m_path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader); // I would really like the ability to assign shaders to objects, keep things together. But accessing an existing shader is better. Override maybe?

private:
	void SetupMesh();

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
};

