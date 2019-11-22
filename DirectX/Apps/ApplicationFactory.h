#pragma once
#include "IApplication.h"
#include "ModelsRenderingApp.h"
#include "DynamicLightingApp.h"

#include <memory>

enum class APP_ID
{
	MODELS_RENDERING,
	DYNAMIC_LIGHTING
};

class ApplicationFactory
{
public:
	std::unique_ptr<IApplication> Create(APP_ID ID) const
	{
		switch (ID)
		{
		case APP_ID::MODELS_RENDERING:
			return std::make_unique<ModelsRenderingApp>();
		case APP_ID::DYNAMIC_LIGHTING:
			return std::make_unique<DynamicLightApp>();
		default:
			throw std::exception("Invalid ID. No App available");
		}
	}
};