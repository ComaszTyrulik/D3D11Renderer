#pragma once
#include "IApplication.h"

#include <string>
#include <memory>
#include <random>
#include <cmath>

#include "../Window.h"
#include "../Context.h"
#include "../vendor/Chili/Timer.h"
#include "../VertexBuffer.h"
#include "../PixelShader.h"
#include "../Models/Model.h"
#include "../Models/ModelInstance.h"
#include "../Pipeline.h"
#include "../IConstantBuffer.h"
#include "../DepthStencilBuffer.h"

#include "../vendor/BEG/ImageLoader.h"
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/matrix_transform.hpp"

using namespace d3dt;

class AbstractApp
	: public IApplication
{
public:
	virtual ~AbstractApp() = default;
	virtual int Start() override;

protected:
	virtual void DoFrame() = 0;

protected:
	std::unique_ptr<d3dt::Window> m_window;
	std::shared_ptr<d3dt::Context> m_context;
	ChiliTimer m_timer;

	std::shared_ptr<Pipeline> m_pipeline = nullptr;

	std::shared_ptr<VertexShader> m_vertexShader = nullptr;
	std::shared_ptr<PixelShader> m_pixelShader = nullptr;
	std::shared_ptr<DepthStencilBuffer> m_depthStencilBuffer = nullptr;

	std::mt19937 rng{ std::random_device{}() };
	std::uniform_int_distribution<int> sdist{ 0,4 };
	std::uniform_real_distribution<float> adist{ 0.0f,std::_Pi * 2.0f };
	std::uniform_real_distribution<float> ddist{ 0.0f,std::_Pi * 0.5f };
	std::uniform_real_distribution<float> odist{ 0.0f,std::_Pi * 0.08f };
	std::uniform_real_distribution<float> rdist{ -20.0f,20.0f };
	std::uniform_real_distribution<float> bdist{ -20.0f,20.0f };
	std::uniform_real_distribution<float> cdist{ 10.0f,50.0f };
};