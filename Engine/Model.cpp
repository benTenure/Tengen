#include "Model.h"

#include <stdio.h>
#include <utility>

Model::Model()
	: m_meshes()
	, m_materials()
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

	for (auto itr = m_materials.begin(); itr != m_materials.end(); ++itr)
	{
		delete itr->second;
	}
	m_materials.clear();
}

void Model::Draw(Shader& shader, std::unordered_map<std::string, Material*>& materials)
{
	for (auto mesh : m_meshes)
	{
		mesh->Draw(shader, materials);
	}
}

std::unordered_map<std::string, Material*> Model::GetMaterialMap()
{
	return m_materials;
}

void Model::LoadModel(const std::filesystem::path &path)
{
	std::string strPath = path.string();
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(strPath, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_FlipUVs );

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
		aiMesh* loadedMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh* mesh = ProcessMesh(loadedMesh);

		if (scene->HasMaterials())
		{
			aiMaterial* material = scene->mMaterials[loadedMesh->mMaterialIndex];
			ProcessMaterial(material);

			mesh->SetMaterialName(material->GetName().C_Str());
		}
	
		m_meshes.push_back(mesh);
	}

	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh* Model::ProcessMesh(aiMesh* mesh)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

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

			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_texCoords = vec;
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

	return new Mesh(vertices, indices);
}

void Model::ProcessMaterial(aiMaterial* aiMat)
{
	std::string name(aiMat->GetName().C_Str());

	if (m_materials.find(name) != m_materials.end())
	{
		return;
	}

	// Diffuse Map
	Texture* diffuseMap = CreateTexture(aiMat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);

	// Roughness Map
	Texture* roughnessMap = CreateTexture(aiMat, aiTextureType_DIFFUSE_ROUGHNESS, TextureType::ROUGHNESS);

	// Normal Map
	Texture* normalMap = CreateTexture(aiMat, aiTextureType_NORMALS, TextureType::NORMAL);

	// Shininess Value
	float shininess = 0.0f;
	aiMat->Get(AI_MATKEY_SHININESS, shininess);

	Material* myMat = new Material(name, diffuseMap, roughnessMap, normalMap, shininess);
	m_materials[name] = myMat;
}

Texture* Model::CreateTexture(aiMaterial* aiMat, aiTextureType aiType, TextureType textureType)
{
	//aiString filename;
	//aiMat->GetTexture(aiType, aiMat->GetTextureCount(aiType), &filename);

	// Multiple maps of the same kind are not a thing in Tengen. This should always be true
	// and only ever pull the first texture of the wanted type
	if (aiMat->GetTextureCount(aiType) > 0)
	{
		aiString filename;
		aiGetMaterialTexture(aiMat, aiType, 0, &filename);
		std::filesystem::path pathToTexture(m_directory + '/' + filename.C_Str());
		return new Texture(textureType, pathToTexture);		
	}
	/*
	else
	{
		UseRelevantBackupColorInstead();
	}
	*/

	return nullptr;
}