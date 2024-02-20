#include "MeshComponent.h"

#include "Runtime/GameObject.h"
#include "MaterialComponent.h"

MeshComponent::MeshComponent(Model* model, GameObject* parent)
	: Component(parent, ComponentType::MESH)
	, m_model(model)
{}

void MeshComponent::Process(float deltaTime)
{
	if (m_model)
	{
		// Need to reference the material to bring in
		MaterialComponent* mat = static_cast<MaterialComponent*>(m_parent->GetComponent(ComponentType::MATERIAL));
		
		if (mat)
		{
			Shader shader = mat->GetShader();
			std::unordered_map<std::string, Material*> mats = mat->GetMaterials();
			m_model->Draw(shader, mats);
		}
	}
	
}

void MeshComponent::SetModel(Model* model)
{
	m_model = model;
}
