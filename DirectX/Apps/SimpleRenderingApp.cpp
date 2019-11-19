#include "SimpleRenderingApp.h"

void SimpleRenderingApp::Init(const std::string& name, int positionX, int positionY, int width, int height)
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

	const std::size_t numberOfCubes = 100;
	m_models.resize(numberOfCubes);
	for (std::size_t i = 0; i < numberOfCubes; ++i)
	{
		m_models[i] =
			Model::CreateCube(
				{ rdist(rng), bdist(rng), cdist(rng) },
				{ 1.0f, 1.0f, 1.0f },
				{ odist(rng), odist(rng), odist(rng) },
				texture
			);
		m_renderer->AddModel(m_models[i]);
	}

	glm::mat4 vpMatrix =
		glm::perspectiveLH(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 200.f)*
		glm::lookAtLH(glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_renderer->SetViewProjectionMatrix(vpMatrix);
}

void SimpleRenderingApp::CreateShaders()
{
	InputLayout layout(
		{
			{ "POSITION", DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT },
			{ "COLOR", DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT },
			{ "TEXCOORDS", DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT },
		}
	);
	m_vertexShader =
		m_context->CreateVertexShader(
			L"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\x64\\Debug\\SingleModelMatrixPerObjectVertexShader.cso",
			layout
		);

	m_pixelShader = m_context->CreatePixelShader(L"C:\\Users\\Tomek\\Documents\\Projects\\C++\\DirectX\\x64\\Debug\\PixelShader.cso");
}

void SimpleRenderingApp::DoFrame()
{
	for (auto& model : m_models)
	{
		model->RotatePitchYawRoll(adist(rng), ddist(rng), odist(rng));
		model->RotateThetaPhiChi(adist(rng), ddist(rng), odist(rng));
		model->Update();
	}

	m_renderer->Render({ 0.1f, 0.1f, 0.1f });
}
