#pragma once

#include <glm/glm.hpp>

class Transform
{
public:
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;
};

