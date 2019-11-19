#pragma once
#include "Color.h"
#include "vendor/glm/glm/glm.hpp"

namespace d3dt
{
	struct Vertex
	{
		glm::vec3 position;
		Color color;
		glm::vec2 texCoords;
		glm::vec3 normals;
	};
}