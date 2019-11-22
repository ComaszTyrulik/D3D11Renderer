#pragma once
#include "glm/glm.hpp"

namespace d3dt
{
	struct Light
	{
		glm::vec4 ambientColor;
		glm::vec4 diffuseColor;
		glm::vec4 specularColor;

		glm::vec3 position;
	};
}