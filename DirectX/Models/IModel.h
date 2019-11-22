#pragma once
#include "../Vertex.h"
#include "../IPipeline.h"
#include "../Material.h"

#include <vector>

namespace d3dt
{
	class IModel
	{
	public:
		virtual ~IModel() = default;
		virtual void Draw(IPipeline* pipeline, const glm::mat4& modelMatrix, bool useTextures) const = 0;
		virtual void SetMaterial(Material material) = 0;
	};
}