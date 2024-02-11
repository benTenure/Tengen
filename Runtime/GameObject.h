#pragma once

#include <vector>

#include "Components/Component.h"
#include "Engine/Transform.h"

class GameObject
{
public:
	virtual void Input(float deltaTime);
	virtual void Process(float deltaTime);

	// The only way for components to be made.
	void AddComponent(Component* component);
	void DeleteComponent(ComponentType type);
	Component* GetComponent(ComponentType type = ComponentType::NONE);

	Transform GetTransform();
	void SetTransform(Transform transform);

private:
	bool DoesComponentExist(ComponentType type);

	//ID m_id;
	//GUID m_guid; // look into boost or other options
	
	std::vector<Component*> m_components; //unordered_map for dealing with GO's with lots of components? O(n) really isn't that bad here though
	Transform m_transform;
};