#pragma once

#include <glm/glm.hpp>

#include "Window.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 targetPosition);

	glm::mat4 Update(); // GetCurrentView() might be a better name? Cherno called his GetViewMatrix() which is as good as it gets

	void ProcessInput(Window window, float deltaTime);

	// Used as the callback function for mouse pointer input
	void ProcessMouse(float xoffset, float yoffset);

	// Used as the callback function for the scroll wheel
	void ProcessScrollWheel(double xoffset, double yoffset);

	void SetPosition(glm::vec3 position);
	glm::vec3 GetPosition();

	void SetFront(glm::vec3 front);
	glm::vec3 GetFront();

	void SetUp(glm::vec3 up);
	glm::vec3 GetUp();

	void SetYaw(float yaw);
	float GetYaw();

	void SetPitch(float pitch);
	float GetPitch();

	void SetFOV(float fov);
	float GetFOV();

private:
	glm::vec3 m_position{};
	glm::vec3 m_front{};
	glm::vec3 m_up{};

	float m_yaw{};
	float m_pitch{};
	float m_fov{};
	float m_sensitivity{};
};
