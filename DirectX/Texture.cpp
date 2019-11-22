#include "Texture.h"

namespace d3dt
{
	Texture::Texture(const beu::Image& textureData, wrl::ComPtr<ID3D11DeviceContext> context, wrl::ComPtr<ID3D11Device> device)
		: m_context(context), m_device(std::move(device))
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = textureData.getDimensions().first;
		textureDesc.Height = textureData.getDimensions().second;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.ArraySize = 1;
		textureDesc.MipLevels = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA resourceData = {0};
		resourceData.pSysMem = textureData.getPixels().data();
		resourceData.SysMemPitch = textureData.getDimensions().first * sizeof(unsigned char) * 4;

		wrl::ComPtr<ID3D11Texture2D> texture;
		API_CALL(m_device->CreateTexture2D(&textureDesc, &resourceData, &texture));

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};
		resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = 1;
		resourceViewDesc.Texture2D.MostDetailedMip = 0;
		API_CALL(m_device->CreateShaderResourceView(texture.Get(), &resourceViewDesc, &m_textureResourceView));

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		API_CALL(m_device->CreateSamplerState(&samplerDesc, &m_samplerState));

		const auto result = UuidCreate(&m_id);
		if (result != 0)
		{
			DebugBreak();
		}
	}

	void Texture::Use()
	{
		m_context->PSSetShaderResources(0, 1, m_textureResourceView.GetAddressOf());
		m_context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());
	}
	
	bool Texture::operator==(const Texture& other)
	{
		return m_id == other.m_id;
	}

	bool Texture::operator!=(const Texture& other)
	{
		return m_id != other.m_id;
	}

	bool Texture::operator<(const Texture& other)
	{
		RPC_CSTR thatString, otherString;
		{
			const auto result = UuidToString(&m_id, &thatString);
			if (result != 0)
			{
				DebugBreak();
			}
		}
		{
			const auto result = UuidToString(&other.m_id, &otherString);
			if (result != 0)
			{
				DebugBreak();
			}
		}

		const auto result = memcmp(thatString, otherString, sizeof(GUID));

		RpcStringFree(&thatString);
		RpcStringFree(&otherString);

		return result < 0;
	}
}
