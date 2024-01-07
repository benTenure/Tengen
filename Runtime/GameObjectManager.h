#pragma once

#include <vector>

#include "GameObject.h"
#include "Singleton.h"

/*

What is the purpose of this class?

- This will be the class that manages the lifetime of all GO's in the level.
  It will also provide a method of allowing components to find and access 
  their parent's component list... This is over engineered... Each component
  can just hold a reference to their parent by making it so only an existing
  GO can create its components.

  There might be a use for a GO manager in the future. Maybe for
  something cooler that *actually* needs this

*/

class GameObjectManager : public Singleton
{
public:

private:
	std::vector<GameObject*> m_gameObjectList;
};

