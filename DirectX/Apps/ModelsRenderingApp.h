#pragma once
#include "AbstractApp.h"

class ModelsRenderingApp
	: public AbstractApp
{
public:
	virtual void Init(const std::string& name, int positionX, int positionY, int width, int height) override;

private:
	void CreateShaders();
	virtual void DoFrame() override;

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

	struct ViewerPositionCBufferData
	{
		glm::vec3 position;
	};

private:
	DirectionalLightCBufferData m_dirLight;
	ViewerPositionCBufferData m_viewerPositionCBufferData;

	std::shared_ptr<IConstantBuffer<ViewProjectionCBufferData>> m_vpCBuffer;
	std::shared_ptr<IConstantBuffer<DirectionalLightCBufferData>> m_dirLightBuffer;
	std::shared_ptr<IConstantBuffer<ViewerPositionCBufferData>> m_viewerPositionCBuffer;
	
	std::vector<std::shared_ptr<ModelInstance>> m_models;

	glm::vec3 m_cameraEye = glm::vec3(0.0f, 0.0f, -50.0f);
	glm::vec3 m_frontVector = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	int m_width, m_height;
};