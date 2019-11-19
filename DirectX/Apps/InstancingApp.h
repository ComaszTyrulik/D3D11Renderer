#pragma once
#include "AbstractApp.h"
#include "../Context.h"

class InstancingApp
	: public AbstractApp
{
public:
	// Inherited via IApplication
	virtual void Init(const std::string& name, int positionX, int positionY, int width, int height) override;

private:
	void CreateShaders();
	virtual void DoFrame() override;

private:
	std::unique_ptr<InstancedRenderer> m_instancedRenderer;
	std::vector<std::shared_ptr<ModelInstance>> m_models;
};