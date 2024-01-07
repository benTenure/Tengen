#pragma once

#include <stack>

#include "InputContext.h"

class InputManager
{
public:
	InputManager();

private:
	//InputContext m_currentContext;

	// TODO: maintain a stack of contexts so I can avoid having to re-create a context I'm about to go back to
	// ie. Opening and closing a menu 1000 times in a row (Still a problem for the second context?)
	// Fix: Find and create all relevant contexts at init... sounds like I can just replace the current one that way...
	// std::stack<InputContext> m_inputContexts;
};