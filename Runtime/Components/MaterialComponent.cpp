#include "MaterialComponent.h"

MaterialComponent::MaterialComponent(Shader shader, std::unordered_map<std::string, Material*> materials, GameObject* parent)
	: Component(parent, ComponentType::MATERIAL)
	, m_shader(shader)
	, m_materials(materials)
{}

Shader MaterialComponent::GetShader()
{
	return m_shader;
}

std::unordered_map<std::string, Material*> MaterialComponent::GetMaterials()
{
	return m_materials;
}

Material* MaterialComponent::GetMaterial(std::string materialName)
{
	if (m_materials.find(materialName) != m_materials.end())
	{
		return m_materials[materialName];
	}

	return nullptr;
}
