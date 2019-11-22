#pragma once
#include "vendor/glm/glm/glm.hpp"

namespace d3dt
{
	struct Material
	{
		glm::vec4 ambientColor = glm::vec4(1.0f);
		glm::vec4 diffuseColor = glm::vec4(1.0f);
		glm::vec4 specularColor = glm::vec4(1.0f);
		float shininess = 0.0f;
	};
}