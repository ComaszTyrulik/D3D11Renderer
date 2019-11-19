#include "InputLayout.h"
#include "vendor/BEG/ImageLoader.h"

namespace d3dt
{
	InputLayout::InputLayout(const std::vector<VertexAttribute>& attributes)
	{
		m_elementsDescriptions.reserve(attributes.size());
		for (const auto& attribute : attributes)
		{
			D3D11_INPUT_ELEMENT_DESC attributeDescriptor;
			attributeDescriptor.SemanticName = attribute.name;
			attributeDescriptor.SemanticIndex = attribute.semanticsIndex;
			attributeDescriptor.Format = attribute.dataFormat;
			attributeDescriptor.InputSlot = attribute.intputSlot;
			attributeDescriptor.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			attributeDescriptor.InputSlotClass = static_cast<D3D11_INPUT_CLASSIFICATION>(BeastEngine::Utils::getUnderlyingType(attribute.type));
			attributeDescriptor.InstanceDataStepRate = (attribute.type == VertexAttributeType::INSTANCE ? 1 : 0);

			m_elementsDescriptions.push_back(attributeDescriptor);
		}
	}
}
