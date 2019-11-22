#pragma once
#include "WinInclude.h"

#include "vendor/BEG/ImageLoader.h"
#include "vendor/BEG/Image.h"

#include <string>
#include <memory>
#include <vector>
#include <d3d11.h>
#include <rpc.h>

namespace d3dt
{
	namespace beu = BeastEngine::Utils;

	class TexturesArray
	{
	public:
		TexturesArray(const std::vector<beu::Image>& texturesData, wrl::ComPtr<ID3D11DeviceContext> context, const wrl::ComPtr<ID3D11Device>& device)
			: m_context(std::move(context)), m_numberOfTextures(texturesData.size())
		{
			if (texturesData.empty())
			{
				return;
			}

			D3D11_TEXTURE2D_DESC textureDesc = {};
			textureDesc.Width = texturesData.front().getDimensions().first;
			textureDesc.Height = texturesData.front().getDimensions().second;
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			textureDesc.ArraySize = texturesData.size();
			textureDesc.MipLevels = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;
			textureDesc.CPUAccessFlags = 0;
			textureDesc.MiscFlags = 0;

			std::vector<D3D11_SUBRESOURCE_DATA> resourcesData(texturesData.size());
			for (std::size_t i = 0; i < texturesData.size(); ++i)
			{
				const auto& textureData = texturesData[i];
				resourcesData[i].pSysMem = textureData.getPixels().data();
				resourcesData[i].SysMemPitch = textureData.getDimensions().first * sizeof(unsigned char) * 4;
			}

			wrl::ComPtr<ID3D11Texture2D> texture;
			API_CALL(device->CreateTexture2D(&textureDesc, resourcesData.data(), &texture));

			D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};
			resourceViewDesc.Format = textureDesc.Format;
			resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			resourceViewDesc.Texture2DArray.MipLevels = 1;
			resourceViewDesc.Texture2DArray.MostDetailedMip = 0;
			resourceViewDesc.Texture2DArray.FirstArraySlice = 0;
			resourceViewDesc.Texture2DArray.ArraySize = texturesData.size();
			API_CALL(device->CreateShaderResourceView(texture.Get(), &resourceViewDesc, &m_textureResourceView));

			D3D11_SAMPLER_DESC samplerDesc = {};
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			API_CALL(device->CreateSamplerState(&samplerDesc, &m_samplerState));
		}

		void Use(UINT startSlot)
		{
			if (m_numberOfTextures == 0)
			{
				return;
			}

			m_context->PSSetShaderResources(startSlot, 1, m_textureResourceView.GetAddressOf());
			m_context->PSSetSamplers(startSlot, 1, m_samplerState.GetAddressOf());
		}

		std::uint32_t Count() const
		{
			return m_numberOfTextures;
		}

	private:
		std::uint32_t m_numberOfTextures;

		wrl::ComPtr<ID3D11DeviceContext> m_context;

		wrl::ComPtr<ID3D11ShaderResourceView> m_textureResourceView;
		wrl::ComPtr<ID3D11SamplerState> m_samplerState;
	};
}