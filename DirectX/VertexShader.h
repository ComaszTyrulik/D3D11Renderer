#pragma once
#include "WinInclude.h"
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <string>
#include "InputLayout.h"
#include "IConstantBuffer.h"
#include "D3D11ConstantBuffer.h"

namespace d3dt
{
	namespace wrl = Microsoft::WRL;
	
	class VertexShader
	{
	public:
		VertexShader(const WCHAR* filepath, wrl::ComPtr<ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device, const InputLayout& inputLayout);
		void Use();

		template <typename T>
		void SetConstantBuffer(const IConstantBuffer<T>& buffer, UINT slot = 0)
		{
			const d3d11::D3D11ConstantBuffer<T>& d3dBuffer = static_cast<const d3d11::D3D11ConstantBuffer<T>&>(buffer);
			m_context->VSSetConstantBuffers(slot, 1, d3dBuffer.GetBufferPtr().GetAddressOf());
		}

	private:
		wrl::ComPtr<ID3DBlob> m_shaderByteCode;
		wrl::ComPtr<ID3D11VertexShader> m_shader;
		wrl::ComPtr<ID3D11DeviceContext> m_context;
		wrl::ComPtr<ID3D11Device> m_device;
		wrl::ComPtr<ID3D11InputLayout> m_inputLayout;
	};
}