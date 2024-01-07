#pragma once

#include "Component.h"
#include "Engine/Shader.h"

// This is only a glorified shader wrapper ATM. More functionality to come!
class MaterialComponent : public Component
{
public:
	MaterialComponent() = delete;
	MaterialComponent(Shader shader, GameObject* parent);

	Shader GetShader();

private:
	Shader m_shader;
};

