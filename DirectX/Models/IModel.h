#pragma once
#include "../Vertex.h"

#include <vector>

namespace d3dt
{
	class IModel
	{
	public:
		virtual ~IModel() = default;
		
		virtual const std::vector<Vertex>& Vertices() const = 0;
		virtual const std::vector<unsigned int>& Indices() const = 0;
		
		virtual void Draw(IPipeline* pipeline, const glm::mat4& modelMatrix) const = 0;
	};
}