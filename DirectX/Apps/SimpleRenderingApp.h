#pragma once
#include "AbstractApp.h"

class SimpleRenderingApp
	: public AbstractApp
{
public:
	virtual void Init(const std::string& name, int positionX, int positionY, int width, int height) override;

private:
	void CreateShaders();
	virtual void DoFrame() override;

private:
	std::unique_ptr<BatchRenderer> m_renderer;
	std::vector<std::shared_ptr<ModelInstance>> m_models;
};