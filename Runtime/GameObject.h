#pragma once

#include <vector>

#include "Components/Component.h"

class GameObject
{
public:
	GameObject();

	void Create() {} // good place to catch errors early

	void Input(float deltaTime)
	{
		/*for (unsigned int i = 0; i < m_components.size(); i++)
		{
			m_components[i].Input(deltaTime);
		}*/

		/*for (auto& component : m_components)
		{
			component.Input(deltaTime);
		}*/
	}

	void Process(float deltaTime)
	{
		/*for (unsigned int i = 0; i < m_components.size(); i++)
		{
			m_components[i].Process(deltaTime);
		}*/

		/*for (auto& component : m_components)
		{
			component.Process(deltaTime);
		}*/
	}

	virtual void Destroy() {}

private:
	unsigned int m_id;
	std::vector<Component> m_components;

	//GUID m_guid; // look into boost or other options
};