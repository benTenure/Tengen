#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"

using namespace MeshLoader;

class Model
{
public:
	Model()
		: m_meshes()
		, m_loadedTextures()
		, m_directory("")
		, m_applyGammaCorrection(false)
	{}

	Model(const char* path, bool gamma = false)
		: m_applyGammaCorrection(gamma)
	{
		LoadModel(path);
	}

	void Draw(Shader& shader);

private:
	void LoadModel(const std::string &path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	
	std::vector<Mesh> m_meshes;
	std::vector<Texture> m_loadedTextures;
	std::string m_directory;
	bool m_applyGammaCorrection;
};
