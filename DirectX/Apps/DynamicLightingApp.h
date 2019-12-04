#pragma once
#include "AbstractApp.h"

#include "../Camera/FPPCamera.h"
#include "../Material.h"
#include "../Lights/Light.h"

class DynamicLightApp
	: public AbstractApp
{
public:
	virtual void Init(const std::string& name, int positionX, int positionY, int width, int height) override;

private:
	virtual void DoFrame() override;
	void ProcessInput();
	void ProcessLights();
	void CreateShaders();

private:
	struct ViewProjectionCBufferData
	{
		glm::mat4 data = glm::mat4(1.0f);
	};

	struct DirectionalLightCBufferData
	{
		glm::vec4 direction;
		glm::vec4 color;
	};

	struct LightCBufferData
	{
		Light data;
	};

	struct ViewerPositionCBufferData
	{
		glm::vec3 position;
	};

private:
	ViewerPositionCBufferData m_viewerPosition;
	LightCBufferData m_light;

	std::shared_ptr<IConstantBuffer<ViewProjectionCBufferData>> m_vpCBuffer;
	std::shared_ptr<IConstantBuffer<ViewerPositionCBufferData>> m_viewerPositionCBuffer;
	std::shared_ptr<IConstantBuffer<LightCBufferData>> m_lightCBuffer;

	std::vector<std::shared_ptr<ModelInstance>> m_models;

	glm::vec3 m_cameraEye = glm::vec3(0.0f, 0.0f, -50.0f);
	glm::vec3 m_frontVector = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	std::unique_ptr<FPPCamera> m_camera;

	int m_width, m_height;
};