#pragma once
#include "WinInclude.h"

#include <vector>
#include <d3d11.h>
#include <wrl.h>

#include "Vertex.h"

namespace d3dt
{
	namespace wrl = Microsoft::WRL;

	class VertexBuffer
	{
		friend class Context;

	public:
		template <typename VertexType>
		VertexBuffer(const std::vector<VertexType>& data, wrl::ComPtr<ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device)
			: m_stride(sizeof(VertexType)), m_offset(0), m_context(context), m_device(device)
		{
			D3D11_BUFFER_DESC bufferDescriptor = {};
			bufferDescriptor.ByteWidth = static_cast<UINT>(data.size() * sizeof(VertexType));
			bufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
			bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDescriptor.MiscFlags = 0;
			bufferDescriptor.StructureByteStride = sizeof(VertexType);

			D3D11_SUBRESOURCE_DATA bufferData = {};
			bufferData.pSysMem = data.data();

			API_CALL(m_device->CreateBuffer(&bufferDescriptor, &bufferData, &m_bufferPtr));
		}

		void Bind(UINT inputSlot = 0) const
		{
			m_context->IASetVertexBuffers(inputSlot, 1, m_bufferPtr.GetAddressOf(), &m_stride, &m_offset);
		}

		template <typename VertexType>
		void Update(const std::vector<VertexType>& data)
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			m_context->Map(m_bufferPtr.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
			memcpy(subresource.pData, data.data(), data.size() * sizeof(VertexType));
			m_context->Unmap(m_bufferPtr.Get(), 0);
		}

	private:
		UINT m_stride;
		UINT m_offset;

		wrl::ComPtr<ID3D11Buffer> m_bufferPtr;
		wrl::ComPtr<ID3D11DeviceContext> m_context;
		wrl::ComPtr<ID3D11Device> m_device;
	};
}