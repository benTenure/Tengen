//#pragma once
//
//#include <unordered_map>
//#include <string>
//#include <tuple>
//#include <vector>
//
//#include "glm/glm.hpp"
//
///*
//
//What is Input?
//-------------------
//Action::Jump
//Value::Boolean
//Input::Key::Space
//Input::Gamepad::XBOX::A
//Input::Gamepad::PS::X
//
//Different states of input
//-----------------------------
//Pressed -> Down, Up (Down in the previous frame and now not)
//Hold Down
//
//Input Events
//------------------------
//
//
//I need to be able to have any of those inputs trigger the action
//
//This is getting very complicated... and it doesn't even work? I'm going crazy fast... need to make something that works
//
//It's common to pack all possible buttons into an unsigned int and then using bit masks to get the relevant input and it's state
//
//*/
//
//enum class Platform : uint8_t
//{
//	NONE = 0,
//	WINDOWS,
//	XBSX,
//	PS5
//};
//
//// Default state of button should be up and off
//enum class ButtonState : uint8_t
//{
//	None = 0,
//	Up,
//	Down,
//	//Held // Need to figure out how to deal with DOWN + HELD being true at the same time because that makes sense? Maybe not for consumption though...
//};
//
//union InputValue
//{
//	ButtonState buttonValue; // 1 byte
//	float triggerValue;		 // 4 bytes
//	glm::vec2 analogValue;	 // 8 bytes? This will probably be the size of the union either way
//};
//
//// Button or Analog or Trigger
//class Input
//{
//public:
//	Input()
//		: m_platform(Platform::WINDOWS)
//	{}
//
//	virtual bool operator==(const Input& rhs) const = 0;
//	virtual InputValue GetValue() = 0;
//
//	Platform GetPlatform();
//	void SetPlatform(Platform platform);
//
//protected:
//	Platform m_platform;
//	InputValue m_value;
//};
//
//class Button : public Input
//{
//public:
//	bool operator ==(const Input& rhs) const override
//	{
//		return true;
//		//return (this->m_value.buttonValue == rhs.m_value.buttonValue);
//	}
//
//	InputValue GetValue() override
//	{
//		return m_value;
//	}
//
//private:
//
//};
//
//// This would be alphabetical unlike the controllers
//enum class Key : uint8_t
//{
//	None = 0,
//	Space,
//	W,
//	A,
//	S,
//	D
//};
//
//// It's clear that picking a platform will be a necessary step in setting up input (M&K, Xbox, PS5) 
//namespace Gamepad
//{
//	namespace XBOX
//	{
//		enum class Button : uint8_t
//		{
//			None = 0,
//			A,
//			B,
//			X,
//			Y
//		};
//	}
//
//	namespace PS
//	{
//		enum class Button : uint8_t
//		{
//			None = 0,
//			CROSS,
//			CIRCLE,
//			SQUARE,
//			TRIANGLE
//		};
//	}
//}
//
///*
//
//UX Stuff for in editor
//----------------------
//
//When creating a mapping there should be an image that shows up to let you pick the input you want in the relevant context (keyboard, controller, etc)
//
//It can also show you which inputs are taken in that context by highlighting them or graying them out
//
//A search bar would be good too. It would be a list of all possible inputs on that platform
//
//*/
//
//// An action is just a name and every action will inherit from this.
//// Create a Jump Action will 
//class Action
//{
//public:
//	Action()
//		: m_name("")
//	{}
//
//	Action(std::string name)
//		: m_name(name)
//	{}
//
//	std::string GetName()
//	{
//		return m_name;
//	}
//
//protected:
//	// UUID m_ID;
//	std::string m_name;
//};
//
//class InputContext
//{
//public:
//	InputContext() {}
//
//	// Load Mapping from File
//	// LoadMap(std::path mappingPath) 
//
//	void AddMappping(Action action, Input input)
//	{
//		// Need a check + warning for when the action is already mapped in the current context to that input
//
//		auto it = m_InputMappings.find(action);
//
//		if (it != m_InputMappings.end())
//		{
//			for (Input& i : m_InputMappings[action])
//			{
//				if (i == input)
//				{
//					// print something or push a warning about trying to map to an existing input
//					return;
//				}
//			}
//
//			m_InputMappings[action].push_back(input);
//		}
//		else
//		{
//			std::vector<Input> newInput;
//			newInput.push_back(input);
//			m_InputMappings.insert(action, newInput);
//		}
//	}
//
//	// Could be useful for needing to reset after player updates input config themselves
//	void ClearMappings()
//	{
//		m_InputMappings.clear();
//	}
//
//private:
//	//We create a new mapping by having choosing an action to add (should they be called events?) 
//	std::unordered_map<Action, std::vector<Input>> m_InputMappings;
//};