#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window
{
public:
	Window(std::string windowName);
	
	void ProcessInput();
	bool CloseWindow();
	void SwapBuffers();
	GLFWwindow* GetWindowHandle();
	void Exit();

	// Must be static to be used by the callback function
	static void APIENTRY PrintDebugMessages(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	static constexpr int WINDOW_DEFAULT_WIDTH = 1920;
	static constexpr int WINDOW_DEFAULT_HEIGHT = 1080;

private:
	GLFWwindow* m_windowHandle = nullptr;
	std::string m_windowName = "Window";
};

