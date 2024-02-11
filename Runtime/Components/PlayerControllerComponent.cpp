#include "PlayerControllerComponent.h"

#include "Engine/Transform.h"
#include "Runtime/GameObject.h"

PlayerControllerComponent::PlayerControllerComponent(GameObject* parent)
	: Component(parent, ComponentType::PLAYERCONTROLLER)
	, m_moveSpeed(0)
{
}

void PlayerControllerComponent::Input(float deltaTime)
{
	Transform transform = m_parent->GetTransform();



}

