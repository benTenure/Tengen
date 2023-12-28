#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <string>
#include <vector>

#include "Shader.h"
#include "Mesh.h"

// There must be a solution to this double namespace nonsense
typedef std::filesystem::path Path;

class Model
{
public:
	Model()
		: m_meshes()
		, m_loadedTextures()
		, m_directory("")
		, m_applyGammaCorrection(false)
	{}

	Model(Path path, bool gamma = false)
		: m_applyGammaCorrection(gamma)
	{
		LoadModel(path);
	}

	void Draw(Shader& shader);

private:
	void LoadModel(const Path &path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
	
	std::vector<Mesh> m_meshes;
	std::vector<Texture> m_loadedTextures;
	std::string m_directory;
	bool m_applyGammaCorrection;
};
