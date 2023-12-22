#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <stb/stb_image.h>

#include "Shader.h"

constexpr unsigned int MAX_BONE_INFLUENCE = 4;

// TODO: Make these actual classes with functions and constructors?? Encapsulate it all in a namespace for now so that 
//		 I only need to be in one file but I can get some good OOP action going

namespace MeshLoader
{
	struct Vertex
	{
		Vertex()
			: m_position(glm::vec3(0.0f))
			, m_normal(glm::vec3(0.0f))
			, m_uvCoords(glm::vec3(0.0f))
		{}

		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_uvCoords;
		glm::vec3 m_tangent;
		glm::vec3 m_biTangent;

		//bone indexes which will influence this vertex
		int m_boneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_weights[MAX_BONE_INFLUENCE];
	};

	struct Texture
	{
		Texture()
			: m_id(0)
			, m_type("")
			, m_path("")
		{}

		unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);

		unsigned int m_id;
		std::string m_type; // Enum with toString might be better
		std::string m_path;
	};

	class Mesh
	{
	public:
		// Please don't keep these public like this :'(
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<Texture> m_textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void Draw(Shader& shader); // I would really like the ability to assign shaders to objects, keep things together. But accessing an existing shader is better. Override maybe?

	private:
		unsigned int m_VAO;
		unsigned int m_VBO;
		unsigned int m_EBO;

		void SetupMesh();
	};
}

