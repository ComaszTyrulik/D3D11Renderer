#pragma once
#include "IPipeline.h"

#include <vector>
#include <memory>

namespace d3dt
{
	class Batch2
	{
	public:
		Batch2(UINT firstIndexLocation, std::shared_ptr<Texture> texture)
			: m_firstIndexLocation(firstIndexLocation), m_texture(std::move(texture))
		{
		}

		void Add(UINT indexCount)
		{
			m_indicesCount += indexCount;
		}

		void Draw(const std::shared_ptr<IPipeline>& pipeline) const
		{
			m_texture->Use();
			pipeline->Draw(m_firstIndexLocation, m_indicesCount);
		}

	private:
		std::shared_ptr<Texture> m_texture = nullptr;
		UINT m_firstIndexLocation = 0;
		UINT m_indicesCount = 0;
	};
}