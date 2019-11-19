#include "VertexShader.h"

namespace d3dt
{
	VertexShader::VertexShader(const WCHAR* filepath, wrl::ComPtr< ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device, const InputLayout& inputLayout)
		: m_context(std::move(context)), m_device(std::move(device))
	{
		D3DReadFileToBlob(filepath, &m_shaderByteCode);
		m_device->CreateVertexShader(m_shaderByteCode->GetBufferPointer(), m_shaderByteCode->GetBufferSize(), nullptr, &m_shader);
		m_device->CreateInputLayout(
			inputLayout.Get().data(),
			static_cast<UINT>(inputLayout.Get().size()),
			m_shaderByteCode->GetBufferPointer(),
			m_shaderByteCode->GetBufferSize(),
			&m_inputLayout
		);
	}

	void VertexShader::Use()
	{
		m_context->IASetInputLayout(m_inputLayout.Get());
		m_context->VSSetShader(m_shader.Get(), nullptr, 0);
	}
}
