#include "Model.h"

#include <stdio.h>
#include <utility>

Model::Model()
	: m_meshes()
	, m_loadedTextures()
	, m_directory("")
	, m_applyGammaCorrection(false)
{}


Model::Model(std::filesystem::path path, bool gamma)
	: m_applyGammaCorrection(gamma)
{
	LoadModel(path);
}

Model::~Model()
{
	for (auto mesh : m_meshes)
	{
		delete mesh;
	}

	for (auto texture : m_loadedTextures)
	{
		delete texture;
	}
}

void Model::Draw(Shader& shader)
{
	for (auto mesh : m_meshes)
	{
		mesh->Draw(shader);
	}
}

void Model::LoadModel(const std::filesystem::path &path)
{
	std::string strPath = path.string();
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(strPath, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate );

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = path.parent_path().string();

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	// Vertex Processing
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector(0.0f);

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.m_position = vector;

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_normal = vector;
		}
		
		if (mesh->HasTextureCoords(0))
		{
			glm::vec2 vec(0.0f);

			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_texCoords = vec;

			if (mesh->HasTangentsAndBitangents())
			{
				// tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.m_tangent = vector;

				// bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.m_biTangent = vector;
			}
		}
		else
		{
			vertex.m_texCoords = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	if (scene->HasMaterials())
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture*> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		std::vector<Texture*> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::NORMAL); //aiTextureType_NORMALS seems to be the actual option??
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture*> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::HEIGHT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return new Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType)
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString filename;
		mat->GetTexture(type, i, &filename); 
		std::filesystem::path pathToTexture(m_directory + '/' + filename.C_Str());

		bool skip = false;

		for (unsigned int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (m_loadedTextures[j]->m_path.string().compare(pathToTexture.string()) == 0)
			{
				textures.push_back(m_loadedTextures[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}

		if (!skip)
		{
			Texture* texture = new Texture(textureType, pathToTexture);
			textures.push_back(texture);
			m_loadedTextures.push_back(texture);
		}
	}

	return textures;
}