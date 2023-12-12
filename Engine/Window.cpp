#include "Window.h"

#include <iostream>

Window::Window(std::string windowName)
	: m_windowName(windowName)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	m_windowHandle = glfwCreateWindow(static_cast<int>(WINDOW_DEFAULT_WIDTH), static_cast<int>(WINDOW_DEFAULT_HEIGHT), m_windowName.c_str(), nullptr, nullptr);

	if (!m_windowHandle)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		//return -1;
	}

	glfwMakeContextCurrent(m_windowHandle);
	// glfwSwapInterval(1); // This turns on VSYNC

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		//return -1;
	}

	// Keep the cursor constrained to the window and invisible
	glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window()
{
}

void Window::ProcessInput()
{
	int pressedKey = glfwGetKey(m_windowHandle, GLFW_KEY_ESCAPE);

	// This feels like it should be a switch-statement... I really should try to do that
	if (glfwGetKey(m_windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_windowHandle, true);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_MINUS) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_EQUAL) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(m_windowHandle, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		//MoveRight();
	}
	else if (glfwGetKey(m_windowHandle, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		//MoveLeft();
	}
}

bool Window::CloseWindow()
{
	return glfwWindowShouldClose(m_windowHandle);
}

void Window::SwapBuffers()
{
	// Everything is finished, send to GPU
	glfwSwapBuffers(m_windowHandle);
	glfwPollEvents();
}

GLFWwindow* Window::GetWindowHandle()
{
	return m_windowHandle;
}
