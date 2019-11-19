#include "IndexBuffer.h"

namespace d3dt
{
	IndexBuffer::IndexBuffer(const std::vector<IndicesType>& indices, wrl::ComPtr<ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device)
		: m_context(context), m_device(device)
	{
		D3D11_BUFFER_DESC bufferDescriptor = {};
		bufferDescriptor.ByteWidth = static_cast<UINT>(indices.size() * sizeof(IndicesType));
		bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
		bufferDescriptor.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDescriptor.CPUAccessFlags = 0;
		bufferDescriptor.MiscFlags = 0;
		bufferDescriptor.StructureByteStride = sizeof(IndicesType);

		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = indices.data();

		m_device->CreateBuffer(&bufferDescriptor, &bufferData, &m_bufferPtr);
		m_numberOfIndices = static_cast<IndicesType>(indices.size());
	}

	void IndexBuffer::Bind()
	{
		m_context->IASetIndexBuffer(m_bufferPtr.Get(), INDICES_FORMAT, 0);
	}
	
	unsigned int IndexBuffer::IndiciesCount() const
	{
		return m_numberOfIndices;
	}
}