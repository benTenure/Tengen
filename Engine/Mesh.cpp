#include "Mesh.h"

// Moving vectors like this good? Seem bad? Slow.
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: m_vertices(vertices)
	, m_indices(indices)
	, m_textures(textures)
	, m_VAO(0)
	, m_VBO(0)
	, m_EBO(0)
{
	SetupMesh();
}

Mesh::~Mesh()
{
	//glDeleteVertexArrays(1, &m_VAO);
	//glDeleteBuffers(1, &m_VBO);
	//glDeleteBuffers(1, &m_EBO);
	//glDeleteTextures();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;
	unsigned int normalNum = 1;
	unsigned int heightNum = 1;

	shader.Use();

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
		
		// retrieve texture number (the N in diffuse_textureN)
		std::string number; 
		std::string name = Texture::ToString(m_textures[i].m_type);

		switch (m_textures[i].m_type)
		{
		case TextureType::DIFFUSE:
			number = std::to_string(diffuseNum++);
			break;
		case TextureType::SPECULAR:
			number = std::to_string(specularNum++);
			break;
		case TextureType::NORMAL:
			number = std::to_string(normalNum++);
			break;
		case TextureType::HEIGHT:
			number = std::to_string(heightNum++);
			break;
		default:
			break;
		}

		// now set the sampler to the correct texture unit
		glUniform1i(glGetUniformLocation(shader.GetID(), (name + number).c_str()), i);

		// and finally bind the texture
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}

	glActiveTexture(GL_TEXTURE0);
	
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::AddVertex(Vertex vertex)
{
	m_vertices.push_back(vertex);
}

void Mesh::AddIndex(unsigned int index)
{
	m_indices.push_back(index);
}

void Mesh::AddTexture(Texture texture)
{
	m_textures.push_back(texture);
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

	//// vertex tangent
	//glEnableVertexAttribArray(3);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_tangent));
	//
	//// vertex bitangent
	//glEnableVertexAttribArray(4);
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_biTangent));

	//// ids
	//glEnableVertexAttribArray(5);
	//glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_boneIDs));

	//// weights
	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_weights));

    glBindVertexArray(0);
}

unsigned int Texture::LoadTextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = path;
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, numComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &numComponents, 0);

	if (data)
	{
		GLenum format = GL_NONE;

		if (numComponents == 1)
			format = GL_RED;
		else if (numComponents == 3)
			format = GL_RGB;
		else if (numComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	stbi_image_free(data);

	return textureID;
}

std::string Texture::ToString(TextureType textureType)
{
	std::string result;

	switch (textureType)
	{
	case TextureType::DIFFUSE:
		result = "texture_diffuse";
		break;
	case TextureType::NORMAL:
		result = "texture_normal";
		break;
	case TextureType::ROUGHNESS:
		result = "texture_roughness";
		break;
	case TextureType::SPECULAR:
		result = "texture_specular";
		break;
	case TextureType::AMBIENT_OCCLUSION:
		result = "texture_ambient_occlusion";
		break;
	case TextureType::HEIGHT:
		result = "texture_height";
		break;
	default:
		break;
	}

	return result;
}
