#include "DepthStencilBuffer.h"

namespace d3dt
{
	DepthStencilBuffer::DepthStencilBuffer(UINT width, UINT height, wrl::ComPtr<ID3D11DeviceContext> context, const wrl::ComPtr<ID3D11Device>& device)
	{
		D3D11_TEXTURE2D_DESC bufferDesc;
		bufferDesc.Width = width;
		bufferDesc.Height = height;
		bufferDesc.MipLevels = 1;
		bufferDesc.ArraySize = 1;
		bufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.SampleDesc.Quality = 0;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		device->CreateTexture2D(&bufferDesc, nullptr, &m_bufferPtr);
		device->CreateDepthStencilView(m_bufferPtr.Get(), nullptr, &m_view);
	}
}
