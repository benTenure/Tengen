#pragma once

#include <unordered_map>
#include <string.h>

#include "Singleton.h"
#include "Engine/Material.h"

static unsigned int NUM_OF_MATS = 0;

class MaterialManager : public Singleton
{
public:
	static MaterialHandle GetMaterial(unsigned int id);
	static MaterialHandle CreateMaterial(std::string path);

private:
	std::unordered_map<unsigned int, Material> m_materials;
};

