#include "Camera.h"
#include "App.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	m_position = glm::vec3(0.0f, 0.0f, 10.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_yaw = -90.0f;
	m_pitch = 0.0f;
	m_fov = 45.0f;
	m_sensitivity = 0.1f;
}

Camera::Camera(glm::vec3 lookAt)
{
	// TODO: Supply a desired target for the camera to spawn looking at. Maybe something saved in the level or because we're doing transitions idk
	// TODO: TODO: Is this really needed? Are cameras really going to by programmatically created and dynamically assigned at runtime? Sounds dumb
}

glm::mat4 Camera::Update()
{
	// Done every frame
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessInput(Window window, float deltaTime)
{
	const float cameraSpeed = 10.0f * deltaTime; // adjust accordingly
	if (glfwGetKey(window.GetWindowHandle(), GLFW_KEY_W) == GLFW_PRESS)
	{
		m_position += cameraSpeed * m_front;
	}

	if (glfwGetKey(window.GetWindowHandle(), GLFW_KEY_S) == GLFW_PRESS)
	{
		m_position -= cameraSpeed * m_front;
	}

	if (glfwGetKey(window.GetWindowHandle(), GLFW_KEY_A) == GLFW_PRESS)
	{
		m_position -= glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
	}

	if (glfwGetKey(window.GetWindowHandle(), GLFW_KEY_D) == GLFW_PRESS)
	{
		m_position += glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
	}
	
	if (glfwGetKey(window.GetWindowHandle(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_position += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	}
	
	if (glfwGetKey(window.GetWindowHandle(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_position += cameraSpeed * glm::vec3(0.0f, -1.0f, 0.0f);
	}
}

// Another function when we need to tell the 

void Camera::ProcessMouse(float xoffset, float yoffset)
{
	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	// Camera movement
	glm::vec3 front = {};
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
}

void Camera::ProcessScrollWheel(double xoffset, double yoffset)
{
	m_fov -= (float)yoffset;
	if (m_fov < 1.0f)
		m_fov = 1.0f;
	if (m_fov > 45.0f)
		m_fov = 45.0f;
}

void Camera::SetPosition(glm::vec3 position)
{
	m_position = position;
}

glm::vec3 Camera::GetPosition()
{
	return m_position;
}

void Camera::SetFront(glm::vec3 front)
{
	m_front = front;
}

glm::vec3 Camera::GetFront()
{
	return m_front;
}

void Camera::SetUp(glm::vec3 up)
{
	m_up = up;
}

glm::vec3 Camera::GetUp()
{
	return m_up;
}

void Camera::SetYaw(float yaw)
{
	m_yaw = yaw;
}

float Camera::GetYaw()
{
	return m_yaw;
}

void Camera::SetPitch(float pitch)
{
	m_pitch = pitch;
}

float Camera::GetPitch()
{
	return m_pitch;
}

void Camera::SetFOV(float fov)
{
	m_fov = fov;
}

float Camera::GetFOV()
{
	return m_fov;
}
