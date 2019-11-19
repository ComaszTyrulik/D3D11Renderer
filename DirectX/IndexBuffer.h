#pragma once
#include "WinInclude.h"

#include <vector>
#include <d3d11.h>
#include <wrl.h>

namespace d3dt
{
	namespace wrl = Microsoft::WRL;

	class IndexBuffer
	{
	public:
		using IndicesType = unsigned int;
		static const DXGI_FORMAT INDICES_FORMAT = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;

		IndexBuffer(const std::vector<IndicesType>& indices, wrl::ComPtr<ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device);
		void Bind();
		unsigned int IndiciesCount() const;

	private:
		wrl::ComPtr<ID3D11Buffer> m_bufferPtr;
		wrl::ComPtr<ID3D11DeviceContext> m_context;
		wrl::ComPtr<ID3D11Device> m_device;

		unsigned int m_numberOfIndices = 0;
	};
}