#pragma once

class Component
{
public:
	// Implements each of the required functions so that the component
	// can basically exist on its own and you just have to add it on.

	// GameObjects have 0 functionality on their own. Just an empty
	// container full of nothing. 

	Component()
		: m_parentId(0)
	{}

	Component(unsigned int parentId)
		: m_parentId(parentId)
	{}

	virtual void Create();
	virtual void Input(float deltaTime);
	virtual void Process(float deltaTime);
	virtual void Destroy();

private:
	unsigned int m_parentId; // Might need a way to reference the parent object since components should not work on their own. CANNOT BE NULL/EMPTY
};