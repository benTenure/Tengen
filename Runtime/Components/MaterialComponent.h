#pragma once

#include "Component.h"
#include "Engine/Shader.h"
#include "Engine/Material.h"

// This is only a glorified shader wrapper ATM. More functionality to come!
class MaterialComponent : public Component
{
public:
	MaterialComponent() = delete;
	MaterialComponent(Shader shader, std::unordered_map<std::string, Material*> materials, GameObject* parent);

	Shader GetShader();
	std::unordered_map<std::string, Material*> GetMaterials();
	Material* GetMaterial(std::string materialName);

private:
	Shader m_shader;
	std::unordered_map<std::string, Material*> m_materials;
};

