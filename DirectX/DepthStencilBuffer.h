#pragma once
#include "WinInclude.h"

#include <d3d11.h>

namespace d3dt
{
	namespace wrl = Microsoft::WRL;

	class DepthStencilBuffer
	{
		friend class Context;

	public:
		DepthStencilBuffer(UINT width, UINT height, wrl::ComPtr<ID3D11DeviceContext> context, const wrl::ComPtr<ID3D11Device>& device);

	private:
		wrl::ComPtr<ID3D11DeviceContext> m_context;

		wrl::ComPtr<ID3D11Texture2D> m_bufferPtr;
		wrl::ComPtr<ID3D11DepthStencilView> m_view;
	};
}
