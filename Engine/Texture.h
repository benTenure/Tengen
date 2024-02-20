#pragma once

#include <filesystem>
#include <string>

enum TextureType
{
	NONE = -1,
	DIFFUSE,
	NORMAL,
	ROUGHNESS,
	SPECULAR,
	AMBIENT_OCCLUSION,
	HEIGHT
};

class Texture
{
public:
	Texture()
		: m_id(0)
		, m_type(TextureType::NONE)
		, m_path("")
	{}

	Texture(TextureType type, std::filesystem::path pathToTexture)
		: m_id(0)
		, m_type(type)
		, m_path(pathToTexture)
	{
		LoadTextureFromFile();
	}

	Texture(TextureType type, char* pathToTexture)
		: m_id(0)
		, m_type(type)
		, m_path(pathToTexture)
	{
		LoadTextureFromFile();
	}


	static std::string ToString(TextureType textureType);

	unsigned int m_id;
	TextureType m_type;
	std::filesystem::path m_path;

private:
	void LoadTextureFromFile();
};

//std::string Texture::ToString(TextureType textureType)
//{
//	std::string result;
//
//	switch (textureType)
//	{
//	case TextureType::DIFFUSE:
//		result = "texture_diffuse";
//		break;
//	case TextureType::NORMAL:
//		result = "texture_normal";
//		break;
//	case TextureType::ROUGHNESS:
//		result = "texture_roughness";
//		break;
//	case TextureType::SPECULAR:
//		result = "texture_specular";
//		break;
//	case TextureType::AMBIENT_OCCLUSION:
//		result = "texture_ambient_occlusion";
//		break;
//	case TextureType::HEIGHT:
//		result = "texture_height";
//		break;
//	default:
//		break;
//	}
//
//	return result;
//}
//
//void Texture::LoadTextureFromFile()
//{
//	glGenTextures(1, &m_id);
//
//	int width, height, numComponents;
//	//stbi_set_flip_vertically_on_load(true);
//	unsigned char* data = stbi_load(m_path.string().c_str(), &width, &height, &numComponents, 0);
//
//	if (data)
//	{
//		GLenum format = GL_NONE;
//
//		if (numComponents == 1)
//		{
//			format = GL_RED;
//		}
//		else if (numComponents == 3)
//		{
//			format = GL_RGB;
//		}
//		else if (numComponents == 4)
//		{
//			format = GL_RGBA;
//		}
//
//		glBindTexture(GL_TEXTURE_2D, m_id);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		// Fix for disappearing edges on transparent textures
//		if (numComponents == 4)
//		{
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		}
//		else
//		{
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		}
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << m_path << std::endl;
//	}
//
//	stbi_image_free(data);
//}