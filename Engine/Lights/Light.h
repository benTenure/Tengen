#pragma once

#include <Runtime/GameObject.h>

class Light : public GameObject
{
public:
	void Input(float deltaTime) override {}
	void Process(float deltaTime) override {}

protected:
	glm::vec3 dir;
	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

private:

};