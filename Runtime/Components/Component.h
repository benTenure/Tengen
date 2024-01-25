#pragma once

#include <stdint.h>

// Should an ID be a shared_ptr wrapper? Eh...
// typedef unsigned int ID;

// This is feeling worse the more I look at it
enum class ComponentType : uint8_t
{
	NONE = 0,
	MESH,
	MATERIAL,
	COLLIDER,
	CUBEMAP
};

// Implements each of the required functions so that the component
// can basically exist on its own and you just have to add it on. 

// GameObjects have 0 functionality on their own. Just an empty
// container full of nothing.

// Forward declaration (fix for cyclic dependency)
class GameObject;

class Component
{
public:
	Component() = delete;

	Component(GameObject* parent, ComponentType type = ComponentType::NONE)
		: m_parent(parent)
		, m_componentType(type)
	{}

	virtual void Input(float deltaTime) {}
	virtual void Process(float deltaTime) {}

	ComponentType CheckType() { return m_componentType; }

protected:
	ComponentType m_componentType;

	//ID m_parentId; // Might need a way to reference the parent object since components should not work on their own. CANNOT BE NULL/EMPTY
	// Will components need an ID too? Would there ever be a need to not duplicate components? Is it even possible here?
	
	// Direct reference to parent. Unsafe and shouldn't be used for anything other than getting components
	GameObject* m_parent;
};