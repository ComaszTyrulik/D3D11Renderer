#pragma once
#include "IApplication.h"
#include "InstancingApp.h"
#include "SimpleRenderingApp.h"
#include "ModelsRenderingApp.h"

#include <memory>

enum class APP_ID
{
	INSTANCING,
	SIMPLE_RENDERING,
	MODELS_RENDERING,
};

class ApplicationFactory
{
public:
	std::unique_ptr<IApplication> Create(APP_ID ID) const
	{
		switch (ID)
		{
		case APP_ID::INSTANCING:
			return std::make_unique<InstancingApp>();
		case APP_ID::SIMPLE_RENDERING:
			return std::make_unique<SimpleRenderingApp>();
		case APP_ID::MODELS_RENDERING:
			return std::make_unique<ModelsRenderingApp>();
		default:
			throw std::exception("Invalid ID. No App available");
		}
	}
};