#pragma once

#include "Component.h"

class PlayerControllerComponent : public Component
{
public:
	PlayerControllerComponent() = delete;
	PlayerControllerComponent(GameObject* parent);

	void Input(float deltaTime) override;

private:
	unsigned int m_moveSpeed;
};