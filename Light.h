#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
	Light()
		: m_ambient(glm::vec3(0.0f))
		, m_diffuse(glm::vec3(0.0f))
		, m_specular(glm::vec3(0.0f))
	{}

	virtual void SetAmbient(glm::vec3* ambient) { m_ambient = *ambient; }
	virtual glm::vec3 GetAmbient() { return m_ambient; }

	virtual void SetDiffuse(glm::vec3* diffuse) { m_diffuse = *diffuse; }
	virtual glm::vec3 GetDiffuse() { return m_diffuse; }
	
	virtual void SetSpecular(glm::vec3* specular) { m_specular = *specular; }
	virtual glm::vec3 GetSpecular() { return m_specular; };

private:
	glm::vec3 m_ambient;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
};

class DirectionalLight : public Light
{
public:
	void SetDirection(glm::vec3* direction);
	glm::vec3 GetDirection();

private:
	glm::vec3 m_direction;
};

class PointLight : public Light
{
public:
	void SetPosition(glm::vec3* position);
	glm::vec3 GetPosition();

	void SetConstant(float constant);
	glm::vec3 GetConstant();

	void SetLinear(float linear);
	glm::vec3 GetLinear();

	void SetQuadratic(float quadratic);
	glm::vec3 GetQuadratic();

private:
	glm::vec3 m_position;

	float m_constant;
	float m_linear;
	float m_quadratic;
};

/*

Base this abstract class off what 
 
struct Light {
	//vec3 position; // Don't need this for a directional light, use direction instead
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

Ultimately it feels like this is just going to be different game objects that will
tell the shader how to use the light depending on its type.

Types:

Directional Light:
	- A "universal" light source that hits every fragment the same way regardless of the objects position.
	- Only requires a direction
	- shader change: vec3 lightDir = normalize(-light.direction);

Point Light:
	- A light source like a light bulb where its impact falls off based on the distance between the fragment
	  and the light
	- Requires the position of the light (world space?)
	- shader change is more complex



*/