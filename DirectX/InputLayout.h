#pragma once
#include "WinInclude.h"

#include <d3d11.h>
#include <string>
#include <vector>

namespace d3dt
{
	class InputLayout
	{
	public:
		enum class VertexAttributeType : int
		{
			VERTEX = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA,
			INSTANCE = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA,
		};

		struct VertexAttribute
		{
			const char* name;
			DXGI_FORMAT dataFormat;
			UINT semanticsIndex = 0;
			VertexAttributeType type = VertexAttributeType::VERTEX;
			UINT intputSlot = 0;
		};

		InputLayout(const std::vector<VertexAttribute>& attributes);
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& Get() const
		{
			return m_elementsDescriptions;
		}

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_elementsDescriptions;
	};
}