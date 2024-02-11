#pragma once

#include <glm/glm.hpp>

/*
	This is the digital representation of the 4x4 transformation matrix

	The rotation is currently being handled through Euler angles. I understand
	that Quaternions will need to replace it ASAP but I need practice!
*/
class Transform
{
public:
	Transform();
	Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::mat4 matrix);



private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

	glm::mat4 m_localMatrix;
};

