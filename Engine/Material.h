#pragma once

#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Texture.h"

class Material
{
public:
	Material() = delete;
	Material(std::string name, Texture* diffuseMap, Texture* roughnessMap, Texture* normalMap, float shininess);

	std::string GetName();
	Texture* GetDiffuseMap();
	Texture* GetRoughnessMap();
	Texture* GetNormalMap();
	float GetShininess();
	void SetupUniforms(Shader shader);

private:
	std::string m_name;
	Texture*	m_diffuseMap;
	Texture*	m_roughnessMap;
	Texture*	m_normalMap;
	float		m_shininess;
};