#include "DynamicLightingApp.h"

void DynamicLightApp::Init(const std::string& name, int positionX, int positionY, int width, int height)
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
				"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\DirectX\\Textures\\doge.png",
				beu::PixelsFormat::BE_RGBA
			);
		texture = m_context->CreateTexture(image);
	}

	d3dt::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };

	m_pipeline = std::make_shared<Pipeline>(m_context);
	m_pipeline->SetVertexShader(m_vertexShader);
	m_pipeline->SetPixelShader(m_pixelShader);
	m_pipeline->SetViewport(Viewport(static_cast<float>(width), static_cast<float>(height)));

	m_models.push_back(
		Model::CreateFromFile(
			"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\DirectX\\ModelsFiles\\nanosuit\\nanosuit.obj",
			m_context,
			texture,
			glm::vec3(0.0f, -15.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(0.0f, 180.0f, 0.0f)
		)
	);
	Material material;
	material.shininess = 16.0f;

	m_models.back()->SetMaterial(std::move(material));

	m_camera = std::make_unique<FPPCamera>(m_cameraEye, m_frontVector, m_upVector, 45.0f, static_cast<float>(width) / static_cast<float>(height));

	m_vpCBuffer = m_context->CreateConstantBuffer(ViewProjectionCBufferData{ m_camera->GetMatrix() });
	m_vertexShader->SetConstantBuffer(*m_vpCBuffer);

	m_viewerPosition.position = m_cameraEye;
	m_viewerPositionCBuffer = m_context->CreateConstantBuffer(m_viewerPosition);

	m_light.data.position = { 20.0f, 5.0f, -10.0f };
	m_light.data.ambientColor = { 1.0f, 1.0f, 1.0f, 0.2f };
	m_light.data.diffuseColor = { 1.0f, 1.0f, 1.0f, 0.5f };
	m_light.data.specularColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_lightCBuffer = m_context->CreateConstantBuffer(m_light);
	m_pixelShader->SetConstantBuffer(*m_lightCBuffer, 1);
}

void DynamicLightApp::DoFrame()
{
	ProcessInput();
	ProcessLights();

	m_pipeline->Prepare({ 0.1f, 0.1f, 0.1f, 1.0f });
	for (auto& model : m_models)
	{
		//model->RotatePitchYawRoll(0.0f, 1.0f, 0.0f);
		model->Update();
		model->Draw(m_pipeline.get(), true);
	}
	m_pipeline->Present();
}

void DynamicLightApp::ProcessInput()
{
	glm::vec3 cameraMovementVector(0.0f);

	if (m_window->kbd.KeyIsPressed('W'))
	{
		cameraMovementVector.z += 1.0f;
	}
	else if (m_window->kbd.KeyIsPressed('S'))
	{
		cameraMovementVector.z -= 1.0f;
	}

	if (m_window->kbd.KeyIsPressed('D'))
	{
		cameraMovementVector.x += 1.0f;
	}
	else if (m_window->kbd.KeyIsPressed('A'))
	{
		cameraMovementVector.x -= 1.0f;
	}

	if (m_window->kbd.KeyIsPressed('E'))
	{
		cameraMovementVector.y += 1.0f;
	}
	else if (m_window->kbd.KeyIsPressed('Q'))
	{
		cameraMovementVector.y -= 1.0f;
	}

	if (m_window->kbd.KeyIsPressed('K'))
	{
		m_camera->Rotate({ 0.0f, -1.0f, 0.0f });
	}
	else if (m_window->kbd.KeyIsPressed('L'))
	{
		m_camera->Rotate({ 0.0f, 1.0f, 0.0f });
	}

	if (m_window->kbd.KeyIsPressed('O'))
	{
		m_camera->Rotate({-1.0f, 0.0f, 0.0f });
	}
	else if (m_window->kbd.KeyIsPressed('P'))
	{
		m_camera->Rotate({ 1.0f, 0.0f, 0.0f });
	}

	m_camera->Move(cameraMovementVector);
	m_camera->Update();

	m_viewerPosition.position = m_camera->GetPosition();
	m_viewerPositionCBuffer->Update(m_viewerPosition);
	m_pixelShader->SetConstantBuffer(*m_viewerPositionCBuffer, 2);

	ViewProjectionCBufferData vp;
	vp.data = m_camera->GetMatrix();

	m_vpCBuffer->Update(vp);
	m_vertexShader->SetConstantBuffer(*m_vpCBuffer);
}

void DynamicLightApp::ProcessLights()
{
	const auto rotation = glm::rotate(glm::mat4(1.0f), 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));

	auto& direction = m_light.data.position;
	direction = rotation * glm::vec4(direction, 1.0f);

	m_lightCBuffer->Update(m_light);
	m_pixelShader->SetConstantBuffer(*m_lightCBuffer, 1);
}

void DynamicLightApp::CreateShaders()
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
			L"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\x64\\Debug\\DirectLightingVertexShader.cso",
			layout
		);

	m_pixelShader = m_context->CreatePixelShader(L"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\x64\\Debug\\DirectLightingPixelShader.cso");
}