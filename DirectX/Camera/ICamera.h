#pragma once
#include <glm/glm.hpp>

namespace d3dt
{
	class ICamera
	{
	public:
		virtual void Update() = 0;

		virtual void Move(const glm::vec3& direction) = 0;
		virtual void Rotate(const glm::vec3& rotation) = 0;
		
		virtual const glm::mat4& GetMatrix() const = 0;
		virtual const glm::vec3& GetPosition() const = 0;
	};
}