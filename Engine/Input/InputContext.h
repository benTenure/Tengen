#pragma once

#include <map>
#include <string>

// Like a button input
class Input
{
public:

private:

};

// What type of input is expected from the input
class InputValue
{
	Value<T>
};


struct Action
{
	std::string m_name;
	Input m_input;
	InputValue m_value;
};

class InputContext
{
public:
	InputContext();

	void AddMappping();

protected:
	// map of actions
	std::map<Input, Action> m_InputMappings;

	/*
	
	What is Input?
	-------------------
	Action::Jump
	Value::Boolean
	Input::Key::Space
	Input::Gamepad::XBOX::A
	Input::Gamepad::PS::X

	I need to be able to have any of those inputs trigger the action

	This is getting very complicated... and it doesn't even work? I'm going crazy fast... need to make something that works

	*/
};