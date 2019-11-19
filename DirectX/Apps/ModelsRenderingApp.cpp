#include "ModelsRenderingApp.h"

void ModelsRenderingApp::Init(const std::string& name, int positionX, int positionY, int width, int height)
{
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
				"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\x64\\Debug\\doge.png",
				beu::PixelsFormat::BE_RGBA
			);
		texture = m_context->CreateTexture(image);
	}

	d3dt::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };

	m_pipeline = std::make_shared<Pipeline>(m_context);
	m_pipeline->SetVertexShader(m_vertexShader);
	m_pipeline->SetPixelShader(m_pixelShader);
	m_pipeline->SetViewport(Viewport(static_cast<float>(width), static_cast<float>(height)));

	m_renderer = std::make_unique<BatchRenderer>(m_pipeline);

	m_models.push_back(
		Model::CreateFromFile(
			"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\DirectX\\ModelsFiles\\suzanne.obj",
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(10.0f, 10.0f, 10.0f),
			glm::vec3(0.0f, 180.0f, 0.0f),
			texture
		)
	);
	
	m_renderer->AddModel(m_models.back());

	glm::mat4 vpMatrix =
		glm::perspectiveLH(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 200.f)*
		glm::lookAtLH(glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_renderer->SetViewProjectionMatrix(vpMatrix);
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
			L"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\x64\\Debug\\LightingVertexShader.cso",
			layout
		);

	m_pixelShader = m_context->CreatePixelShader(L"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\x64\\Debug\\DirectLightingPixelShader.cso");
}

void ModelsRenderingApp::DoFrame()
{
	for (auto& model : m_models)
	{
		model->RotatePitchYawRoll(0.0f, 1.0f, 0.0f);
		model->Update();
	}

	m_renderer->Render({ 0.1f, 0.1f, 0.1f });
}
