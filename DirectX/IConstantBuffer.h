#pragma once
#include <memory>

namespace d3dt
{
	template <typename T>
	class IConstantBuffer
	{
	public:
		virtual ~IConstantBuffer() = default;
		virtual void Update(const T& data) = 0;
	};
}