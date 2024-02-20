#include "Mesh.h"

// Moving vectors like this good? Seem bad? Slow.
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	: m_vertices(vertices)
	, m_indices(indices)
	, m_VAO(0)
	, m_VBO(0)
	, m_EBO(0)
{
	SetupMesh();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Mesh::Draw(Shader& shader, std::unordered_map<std::string, Material*>& materials)
{
	// MATERIAL SHOULD EXIST BY THIS POINT
	materials[m_materialName]->SetupUniforms(shader);

	//unsigned int diffuseNum = 1;
	//unsigned int specularNum = 1;
	//unsigned int normalNum = 1;
	//unsigned int heightNum = 1;

	//shader.Use();

	//for (unsigned int i = 0; i < m_textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
	//	
	//	// retrieve texture number (the N in diffuse_textureN)
	//	std::string number; 
	//	std::string name = Texture::ToString(m_textures[i]->m_type);

	//	switch (m_textures[i]->m_type)
	//	{
	//		case TextureType::DIFFUSE:
	//		{
	//			number = std::to_string(diffuseNum++);
	//			break;
	//		}
	//		case TextureType::SPECULAR:
	//		{
	//			number = std::to_string(specularNum++);
	//			break;
	//		}
	//		case TextureType::NORMAL:
	//		{
	//			number = std::to_string(normalNum++);
	//			break;
	//		}
	//		case TextureType::HEIGHT:
	//		{
	//			number = std::to_string(heightNum++);
	//			break;
	//		}
	//		default:
	//		{
	//			break;
	//		}
	//	}

	//	// now set the sampler to the correct texture unit
	//	glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), i);

	//	// and finally bind the texture
	//	glBindTexture(GL_TEXTURE_2D, m_textures[i]->m_id);
	//}

	//glActiveTexture(GL_TEXTURE0);
	
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetMaterialName(std::string materialName)
{
	m_materialName = materialName;
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
  
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
    
    // vertex texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));

    glBindVertexArray(0);
}

void Texture::LoadTextureFromFile()
{
	glGenTextures(1, &m_id);

	int width, height, numComponents;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(m_path.string().c_str(), &width, &height, &numComponents, 0);

	if (data)
	{
		GLenum format = GL_NONE;

		if (numComponents == 1)
		{
			format = GL_RED;
		}
		else if (numComponents == 3)
		{
			format = GL_RGB;
		}
		else if (numComponents == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Fix for disappearing edges on transparent textures
		if (numComponents == 4)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << m_path << std::endl;
	}

	stbi_image_free(data);
}
