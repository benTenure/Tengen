#include "MaterialComponent.h"

MaterialComponent::MaterialComponent(Shader shader, GameObject* parent)
	: Component(parent, ComponentType::MATERIAL)
	, m_shader(shader)
{}

Shader MaterialComponent::GetShader()
{
	return m_shader;
}
