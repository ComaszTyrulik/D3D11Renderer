#include "PixelShader.h"

d3dt::PixelShader::PixelShader(const WCHAR* filepath, wrl::ComPtr<ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device)
	: m_context(std::move(context)), m_device(std::move(device))
{
	D3DReadFileToBlob(filepath, &m_shaderByteCode);
	m_device->CreatePixelShader(m_shaderByteCode->GetBufferPointer(), m_shaderByteCode->GetBufferSize(), nullptr, &m_shader);
}

void d3dt::PixelShader::Use()
{
	m_context->PSSetShader(m_shader.Get(), nullptr, 0);
}
