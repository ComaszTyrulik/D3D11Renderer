#pragma once
#include "WinInclude.h"
#include <d3d11.h>

namespace d3dt::d3d11
{
	template <typename T>
	class D3D11ConstantBuffer
		: public IConstantBuffer<T>
	{
	public:
		D3D11ConstantBuffer(const T& data, const wrl::ComPtr<ID3D11Device>& device, wrl::ComPtr<ID3D11DeviceContext> context)
			: m_context(std::move(context))
		{
			D3D11_BUFFER_DESC cbDesc;
			cbDesc.ByteWidth = BUFFER_SIZE;
			cbDesc.Usage = D3D11_USAGE_DEFAULT;
			cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cbDesc.CPUAccessFlags = 0;
			cbDesc.MiscFlags = 0;
			cbDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = &data;
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;

			API_CALL(device->CreateBuffer(&cbDesc, &initData, &m_bufferPtr));
		}
		
		virtual void Update(const T& data) override
		{
			m_context->UpdateSubresource(m_bufferPtr.Get(), 0, 0, &data, 0, 0);
		}

		const wrl::ComPtr<ID3D11Buffer>& GetBufferPtr() const
		{
			return m_bufferPtr;
		}

	private:
		static const UINT BUFFER_SIZE_MULTIPLY = 16;
		static const UINT BUFFER_SIZE = ((sizeof(T) + BUFFER_SIZE_MULTIPLY - 1) / BUFFER_SIZE_MULTIPLY) * BUFFER_SIZE_MULTIPLY;

		wrl::ComPtr<ID3D11Buffer> m_bufferPtr;
		wrl::ComPtr<ID3D11DeviceContext> m_context;
	};
}