#include "Context.h"

namespace d3dt
{
	Context::Context(const Window::WindowHandle& hWnd)
	{
		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 0;
		sd.BufferDesc.RefreshRate.Denominator = 0;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = hWnd.Get();
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			D3D11_CREATE_DEVICE_DEBUG,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&m_swapChain,
			&m_device,
			nullptr,
			&m_context
		);

		wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
		m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
		m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
	}
	
	void Context::ClearRenderTargetView(const Color& color)
	{
		m_context->ClearRenderTargetView(m_renderTargetView.Get(), color.Data());

		if (m_depthStencilBuffer != nullptr)
		{
			m_context->ClearDepthStencilView(m_depthStencilBuffer->m_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
		}
	}

	void Context::SetDepthStencilBuffer(std::shared_ptr<DepthStencilBuffer> buffer)
	{
		m_depthStencilBuffer = std::move(buffer);
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilBuffer->m_view.Get());
	}

	void Context::SetViewport(const Viewport& viewport)
	{
		m_context->RSSetViewports(1, &viewport.m_d3dViewport);
	}

	void Context::Draw(UINT firstIndexLocation, UINT numberOfIndices)
	{
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_context->DrawIndexed(numberOfIndices, firstIndexLocation, 0);
	}

	void Context::DrawInstanced(UINT indexCountPerInstance, UINT numberOfInstances, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanceLocation)
	{
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_context->DrawIndexedInstanced(indexCountPerInstance, numberOfInstances, startIndexLocation, baseVertexLocation, startInstanceLocation);
	}
	
	void Context::Present()
	{
		m_swapChain->Present(1, 0);
	}
	
	std::unique_ptr<IndexBuffer> Context::CreateIndexBuffer(const std::vector<unsigned int>& indices)
	{
		return std::make_unique<IndexBuffer>(indices, m_context, m_device);
	}
	
	std::unique_ptr<VertexShader> Context::CreateVertexShader(const WCHAR* filepath, const InputLayout& inputLayout)
	{
		return std::make_unique<VertexShader>(filepath, m_context, m_device, inputLayout);
	}
	
	std::unique_ptr<PixelShader> Context::CreatePixelShader(const WCHAR* filepath)
	{
		return std::make_unique<PixelShader>(filepath, m_context, m_device);
	}
	
	std::unique_ptr<Texture> Context::CreateTexture(const beu::Image& data)
	{
		return std::make_unique<Texture>(data, m_context, m_device);
	}
	
	std::unique_ptr<DepthStencilBuffer> Context::CreateDepthStencilBuffer(UINT width, UINT height)
	{
		return std::make_unique<DepthStencilBuffer>(width, height, m_context, m_device);
	}
	
	std::unique_ptr<TexturesArray> Context::CreateTexturesArray(const std::vector<beu::Image>& texturesData)
	{
		return std::make_unique<TexturesArray>(texturesData, m_context, m_device);
	}
}