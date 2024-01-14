#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <string>
#include <vector>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
	Model();
	Model(std::filesystem::path path, bool gamma = false);
	~Model();

	void Draw(Shader& shader);

private:
	void LoadModel(const std::filesystem::path &path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);

	std::vector<Mesh*> m_meshes;
	std::vector<Texture*> m_loadedTextures;
	std::string m_directory;
	bool m_applyGammaCorrection;
};
