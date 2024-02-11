#include "Material.h"

Material::Material(Texture diffuseMap, Texture roughnessMap, Texture normalMap, float shininess)
	: m_diffuseMap(diffuseMap)
	, m_roughnessMap(roughnessMap)
	, m_normalMap(normalMap)
	, m_shininess(shininess)
{

}
