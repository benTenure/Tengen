#include "PlayerControllerComponent.h"

PlayerControllerComponent::PlayerControllerComponent(GameObject* parent)
	: Component(parent, ComponentType::PLAYERCONTROLLER)
	, m_moveSpeed(0)
{
}

void PlayerControllerComponent::Input(float deltaTime)
{

}

