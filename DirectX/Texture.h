#pragma once
#include "WinInclude.h"
#include <d3d11.h>

#include "vendor/BEG/Image.h"
#include <string>
#include <memory>

#include <rpc.h>

namespace d3dt
{
	namespace beu = BeastEngine::Utils;

	class Texture
	{
	public:
		Texture(const beu::Image& textureData, wrl::ComPtr<ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device);
		void Use();
		bool operator ==(const Texture& other);
		bool operator !=(const Texture& other);
		bool operator <(const Texture& other);

	private:
		wrl::ComPtr<ID3D11DeviceContext> m_context;
		wrl::ComPtr<ID3D11Device> m_device;

		wrl::ComPtr<ID3D11ShaderResourceView> m_textureResourceView;
		wrl::ComPtr<ID3D11SamplerState> m_samplerState;

		UUID m_id;
	};
}