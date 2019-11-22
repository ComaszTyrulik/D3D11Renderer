#pragma once
#include "../IPipeline.h"

namespace d3dt
{
	class IDrawable
	{
	public:
		virtual ~IDrawable() = default;
		virtual void Draw(IPipeline* pipeline, bool useTextures = true) const = 0;
	};
}