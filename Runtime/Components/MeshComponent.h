#pragma once

#include "Component.h"
#include "Engine/Model.h"

/*

This component will absolutely rely on a MaterialComponent for a shader and its related properties
 
*/
class MeshComponent : public Component
{

public:
	MeshComponent() = delete;
	MeshComponent(Model* model, GameObject* parent);

	void Process(float deltaTime) override;

	void SetModel(Model* model);

private:
	Model* m_model;
};

