#include "ModelsRenderingApp.h"

void ModelsRenderingApp::Init(const std::string& name, int positionX, int positionY, int width, int height)
{
	m_width = width;
	m_height = height;
	m_window = std::make_unique<d3dt::Window>(name, positionX, positionY, width, height);

	m_context = std::make_shared<d3dt::Context>(m_window->Handle());
	m_depthStencilBuffer = m_context->CreateDepthStencilBuffer(static_cast<UINT>(width), static_cast<UINT>(height));
	m_context->SetDepthStencilBuffer(m_depthStencilBuffer);

	CreateShaders();

	namespace beu = BeastEngine::Utils;
	std::shared_ptr<Texture> texture = nullptr;
	{
		const auto image =
			beu::ImageLoader::loadImage(
				"C:/Users/Tomek/Documents/Projects/C++/DirectX/DirectX/Textures/doge.png",
				beu::PixelsFormat::BE_RGBA
			);
		texture = m_context->CreateTexture(image);
	}

	d3dt::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };

	m_pipeline = std::make_shared<Pipeline>(m_context);
	m_pipeline->SetVertexShader(m_vertexShader);
	m_pipeline->SetPixelShader(m_pixelShader);
	m_pipeline->SetViewport(Viewport(static_cast<float>(width), static_cast<float>(height)));

	//m_models.push_back(
	//	Model::CreateFromFile(
	//		"C:/Users/Tomek/Documents/Projects/C++/DirectX/DirectX/ModelsFiles/sponza.obj",
	//		m_context,
	//		texture,
	//		glm::vec3(0.0f, -10.0f, -100.0f),
	//		glm::vec3(200.0f, 200.0f, 200.0f),
	//		glm::vec3(0.0f, 90.0f, 0.0f)
	//	)
	//);

	m_models.push_back(
		Model::CreateFromFile(
			"ModelsFiles/nanosuit/nanosuit.obj",
			m_context,
			texture,
			glm::vec3(0.0f, -15.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 180.0f, 0.0f)
		)
	);

	glm::mat4 vpMatrix =
		glm::perspectiveLH(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 400.f)*
		glm::lookAtLH(m_cameraEye, m_cameraEye + m_frontVector, m_upVector);

	m_vpCBuffer = m_context->CreateConstantBuffer(ViewProjectionCBufferData{ vpMatrix });
	m_vertexShader->SetConstantBuffer(*m_vpCBuffer);

	m_dirLight.direction = { -20.0f, 5.0f, -10.0f, 0.0f };
	m_dirLight.color = { 1.0f, 1.0f, 1.0f, 3.0f };

	m_dirLightBuffer = m_context->CreateConstantBuffer(m_dirLight);
	m_pixelShader->SetConstantBuffer(*m_dirLightBuffer, 1);

	m_viewerPositionCBufferData.position = m_cameraEye;
	m_viewerPositionCBuffer = m_context->CreateConstantBuffer(m_viewerPositionCBufferData);
}

void ModelsRenderingApp::DoFrame()
{
	const auto rotation = glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));

	auto& direction = m_dirLight.direction;
	direction = rotation * direction;

	m_dirLightBuffer->Update(m_dirLight);
	m_pixelShader->SetConstantBuffer(*m_dirLightBuffer, 1);

	//if (m_window->IsKeyPressed(VK_UP))
	//{
	//	m_cameraEye.z += 1.0f;
	//}
	//else if (m_window->IsKeyPressed(VK_DOWN))
	//{
	//	m_cameraEye.z -= 1.0f;
	//}

	m_viewerPositionCBufferData.position = m_cameraEye;
	m_viewerPositionCBuffer->Update(m_viewerPositionCBufferData);
	m_pixelShader->SetConstantBuffer(*m_viewerPositionCBuffer, 2);

	glm::mat4 vpMatrix =
		glm::perspectiveLH(glm::radians(45.0f), static_cast<float>(m_width) / static_cast<float>(m_height), 0.1f, 400.f)*
		glm::lookAtLH(m_cameraEye, m_cameraEye + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	ViewProjectionCBufferData vp;
	vp.data = vpMatrix;

	m_vpCBuffer->Update(vp);
	m_vertexShader->SetConstantBuffer(*m_vpCBuffer);

	m_pipeline->Prepare({ 0.1f, 0.1f, 0.1f, 1.0f });
	for (auto& model : m_models)
	{
		//model->RotatePitchYawRoll(0.0f, -0.5f, 0.0f);
		model->Update();

		model->Draw(m_pipeline.get());
	}
	m_pipeline->Present();
}

void ModelsRenderingApp::CreateShaders()
{
	InputLayout layout(
		{
			{ "POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT },
			{ "COLOR", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT },
			{ "TEXCOORDS", DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT },
			{ "NORMALS", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT }
		}
	);
	m_vertexShader =
		m_context->CreateVertexShader(
			L"../x64/Debug/LightingVertexShader.cso",
			layout
		);

	m_pixelShader = m_context->CreatePixelShader(L"../x64/Debug/MultiTexturesPixelShader.cso");
}