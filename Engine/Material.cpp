#include "Material.h"

Material::Material(std::string name, Texture* diffuseMap, Texture* roughnessMap, Texture* normalMap, float shininess)
	: m_name(name)
	, m_diffuseMap(diffuseMap)
	, m_roughnessMap(roughnessMap)
	, m_normalMap(normalMap)
	, m_shininess(shininess)
{
}

std::string Material::GetName()
{
	return m_name;
}

Texture* Material::GetDiffuseMap()
{
	return m_diffuseMap;
}

Texture* Material::GetRoughnessMap()
{
	return m_roughnessMap;
}

Texture* Material::GetNormalMap()
{
	return m_normalMap;
}

float Material::GetShininess()
{
	return m_shininess;
}

void Material::SetupUniforms(Shader shader)
{
	shader.Use();

	// set diffuse
	if (m_diffuseMap)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_diffuseMap->m_id);
		shader.SetInt("material.diffuse", TextureLocation::DIFFUSE_LOCATION);
	}

	// set spec
	//glActiveTexture(GL_TEXTURE1);
	//shader.SetInt("material.specular", m_specularMap->m_id);
	//glBindTexture(GL_TEXTURE_2D, m_specularMap->m_id);

	// set emission
	//glActiveTexture(GL_TEXTURE2);
	//shader.SetInt("material.emission", m_emissionMap->m_id);
	//glBindTexture(GL_TEXTURE_2D, m_emissionMap->m_id);

	// set shiny
	shader.SetFloat("material.shininess", m_shininess);
}
