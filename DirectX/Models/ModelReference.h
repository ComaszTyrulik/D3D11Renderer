#pragma once
#include "IModel.h"

#include <memory>
#include <map>
#include <string>

namespace d3dt
{
	class ModelReference final
	{
	public:
		ModelReference() = default;

		ModelReference(IModel* modelPtr, std::string id)
		{	
			m_id = id;

			if (auto mIt = m_modelsRefCountsMap.find(id); mIt != m_modelsRefCountsMap.end())
			{
				delete modelPtr;

				m_model = mIt->second.first;
				mIt->second.second++;

				return;
			}

			m_model = modelPtr;
			m_modelsRefCountsMap[id] = std::make_pair(modelPtr, COUNT_NEW_DEFAULT);
		}

		ModelReference(const ModelReference& other)
		{
			if (this == &other || m_model == other.m_model || m_id == other.m_id)
			{
				return;
			}

			DecreaseCount();
			m_model = other.m_model;
			m_id = other.m_id;

			IncreaseCount();
		}

		ModelReference(ModelReference&&) = default;

		ModelReference& operator=(const ModelReference& other)
		{
			if (this == &other || m_model == other.m_model || m_id == other.m_id)
			{
				return *this;
			}

			DecreaseCount();
			m_model = other.m_model;
			m_id = other.m_id;

			IncreaseCount();

			return *this;
		}

		~ModelReference()
		{
			if (m_id.empty() || m_model == nullptr)
			{
				return;
			}

			auto& refCount = m_modelsRefCountsMap[m_id].second;
			refCount--;

			if (refCount == COUNT_ZERO)
			{
				delete m_model;

				m_modelsRefCountsMap.erase(m_id);
			}
		}

		IModel* const operator->() const
		{
			return m_model;
		}

		IModel* operator->()
		{
			return m_model;
		}

		bool operator ==(const ModelReference& other)
		{
			return m_id == other.m_id;
		}

		const std::string& ID() const
		{
			return m_id;
		}

	private:
		inline static std::map<std::string, std::pair<IModel*, std::size_t>> m_modelsRefCountsMap;
		
		static const std::size_t COUNT_NEW_DEFAULT = 1;
		static const std::size_t COUNT_ZERO = 0;

	private:
		void IncreaseCount()
		{
			m_modelsRefCountsMap[m_id].second++;
		}

		void DecreaseCount()
		{
			m_modelsRefCountsMap[m_id].second--;
		}

		IModel* m_model = nullptr;
		std::string m_id;
	};
}