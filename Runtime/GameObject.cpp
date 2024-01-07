#include "GameObject.h"

void GameObject::Input(float deltaTime)
{
	for (auto c : m_components)
	{
		c->Input(deltaTime);
	}
}

void GameObject::Process(float deltaTime)
{
	for (auto c : m_components)
	{
		c->Process(deltaTime);
	}
}

void GameObject::AddComponent(Component* component)
{
	if (!component || !DoesComponentExist(component->CheckType()))
	{
		m_components.push_back(component);
	}
}

void GameObject::DeleteComponent(Component* component)
{
	if (component || DoesComponentExist(component->CheckType()))
	{
		/*m_components.erase(
			std::remove(
				m_components.begin(), 
				m_components.end(), 
				[](Component* c, Component* component) 
				{ 
					c->CheckType() == component->CheckType(); 
				}
			)
		);*/

		for (auto itr = m_components.begin(); itr != m_components.end(); itr++)
		{
			if ((*itr)->CheckType() == component->CheckType())
			{
				m_components.erase(itr);
			}
		}
	}
}

Component* GameObject::GetComponent(ComponentType type)
{
	if (!DoesComponentExist(type))
	{
		return nullptr;
	}

	for (auto c : m_components)
	{
		if (c->CheckType() == type)
		{
			return c;
		}
	}

	return nullptr;
}

// Return pointer to component if it does exist? better than re-searching over and over
bool GameObject::DoesComponentExist(ComponentType type)
{
	for (auto c : m_components)
	{
		if (type == c->CheckType())
		{
			return true;
		}
	}
	return false;
}
