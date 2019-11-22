#pragma once
#include "WinInclude.h"

#include "IContext.h"
#include "Window.h"
#include "Color.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Viewport.h"
#include "Texture.h"
#include "DepthStencilBuffer.h"

#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include "TexturesArray.h"

namespace d3dt
{
	namespace wrl = Microsoft::WRL;

	class Context
		: public IContext
	{
	public:
		explicit Context(const Window::WindowHandle& hWnd);
		
		// Rendering
		void ClearRenderTargetView(const Color& color);
		void SetDepthStencilBuffer(std::shared_ptr<DepthStencilBuffer> buffer);
		void SetViewport(const Viewport& viewport);
		void Draw(UINT firstIndexLocation, UINT numberOfIndices);
		void DrawInstanced(UINT indexCountPerInstance, UINT numberOfInstances, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanceLocation);
		void Present();

		// Resources creations
		template<typename VertexType>
		std::unique_ptr<VertexBuffer> CreateVertexBuffer(const std::vector<VertexType>& vertices)
		{
			return std::make_unique<VertexBuffer>(vertices, m_context, m_device);
		}
		
		std::unique_ptr<IndexBuffer> CreateIndexBuffer(const std::vector<unsigned int>& indices);
		std::unique_ptr<VertexShader> CreateVertexShader(const WCHAR* filepath, const InputLayout& inputLayout);
		std::unique_ptr<PixelShader> CreatePixelShader(const WCHAR* filepath);
		std::unique_ptr<Texture> CreateTexture(const beu::Image& data);
		std::unique_ptr<DepthStencilBuffer> CreateDepthStencilBuffer(UINT width, UINT height);
		std::unique_ptr<TexturesArray> CreateTexturesArray(const std::vector<beu::Image>& texturesData);

		template <typename T>
		std::unique_ptr<IConstantBuffer<T>> CreateConstantBuffer(const T& data)
		{
			return std::make_unique<d3d11::D3D11ConstantBuffer<T>>(data, m_device, m_context);
		}

	private:
		wrl::ComPtr<ID3D11Device> m_device;
		wrl::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
		wrl::ComPtr<ID3D11DeviceContext> m_context = nullptr;
		wrl::ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;

		std::shared_ptr<DepthStencilBuffer> m_depthStencilBuffer;
	};
}