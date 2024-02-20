#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>

#include "Shader.h"
#include "Mesh.h"
#include "Material.h"

class Model
{
public:
	Model();
	Model(std::filesystem::path path, bool gamma = false);
	~Model();

	void Draw(Shader& shader, std::unordered_map<std::string, Material*>& materials);
	std::unordered_map<std::string, Material*> GetMaterialMap();

private:
	void LoadModel(const std::filesystem::path &path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh);
	void ProcessMaterial(aiMaterial* material);
	Texture* CreateTexture(aiMaterial* aiMat, aiTextureType type, TextureType textureType);

	std::vector<Mesh*> m_meshes;
	std::unordered_map<std::string, Material*> m_materials;
	std::string m_directory;
	bool m_applyGammaCorrection;
};
