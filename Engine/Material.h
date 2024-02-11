#pragma once

#include <Engine/Mesh.h>

struct MaterialHandle
{
	unsigned int m_referencedMaterialID;
};

class Material
{
public:
	Material() = delete;
	Material(Texture diffuseMap, Texture roughnessMap, Texture normalMap, float shininess);

private:
	unsigned int m_ID;

	Texture m_diffuseMap;
	Texture m_roughnessMap;
	Texture m_normalMap;
	
	float m_shininess;
};

