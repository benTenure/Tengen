#include "CubeMapComponent.h"

CubeMapComponent::CubeMapComponent(std::vector<std::filesystem::path> texturePaths, Shader shader, GameObject* parent)
	: Component(parent, ComponentType::CUBEMAP)
	, m_shader(shader)
	, m_VAO(0)
	, m_VBO(0)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_cubeVertices), &g_cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	glGenTextures(1, &m_texture.m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture.m_id);

	int width, height, numChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < texturePaths.size(); i++)
	{
		data = stbi_load(texturePaths[i].string().c_str(), &width, &height, &numChannels, 0);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMapComponent::Process(float deltaTime)
{
	m_shader.Use();
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture.m_id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

