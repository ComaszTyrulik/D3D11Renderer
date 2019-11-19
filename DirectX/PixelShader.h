#pragma once
#include "WinInclude.h"
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <string>

namespace d3dt
{
	namespace wrl = Microsoft::WRL;

	class PixelShader
	{
	public:
		PixelShader(const WCHAR* filepath, wrl::ComPtr<ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device);
		void Use();

	private:
		wrl::ComPtr<ID3DBlob> m_shaderByteCode;
		wrl::ComPtr<ID3D11PixelShader> m_shader;
		wrl::ComPtr<ID3D11DeviceContext> m_context;
		wrl::ComPtr<ID3D11Device> m_device;
		wrl::ComPtr<ID3D11InputLayout> m_inputLayout;
	};
}