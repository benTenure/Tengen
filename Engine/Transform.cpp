#include "Transform.h"

Transform::Transform()
	: m_position(0.0f)
	, m_rotation(0.0f)
	, m_scale(1.0f)
	, m_localMatrix(1.0f)
{
}

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::mat4 matrix)
	: m_position(position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_localMatrix(matrix)
{
}
