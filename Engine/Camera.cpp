#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	m_position = glm::vec3(0.0f, 0.0f, 10.0f);
	m_front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_yaw = 90.0f;
	m_pitch = 0.0f;
	m_fov = 45.0f;

	m_firstMouse = true;
	m_lastX = 1080; // Get these from window class? static public var's?
	m_lastY = 1080;
}

Camera::Camera(glm::vec3 lookAt)
{
	// TODO: Supply a desired target for the camera to spawn looking at. Maybe something saved in the level or because we're doing transitions idk
}

glm::mat4 Camera::Update()
{
	// Done every frame
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessInput(Window window, float deltaTime)
{
	const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
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
}

// Another function when we need to tell the 

void Camera::ProcessMouse(double xpos, double ypos)
{
	float xposf = static_cast<float>(xpos);
	float yposf = static_cast<float>(ypos);

	if (m_firstMouse) // initially set to true. Still needed???
	{
		m_lastX = xposf;
		m_lastY = yposf;
		m_firstMouse = false;
	}

	float xoffset = xposf - m_lastX;
	float yoffset = m_lastY - yposf; // reversed since y-coordinates range from bottom to top
	m_lastX = xposf;
	m_lastY = yposf;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	// Camera movement
	glm::vec3 direction = {};
	direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	direction.y = sin(glm::radians(m_pitch));
	direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(direction);
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
