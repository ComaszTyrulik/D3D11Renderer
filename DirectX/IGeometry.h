#pragma once
#include <vector>

#include "Vertex.h"
#include "glm/glm.hpp"

namespace d3dt
{
	class IGeometry
	{
	public:
		~IGeometry() = default;

		virtual const std::vector<Vertex>& Vertices() const = 0;
		virtual const std::vector<unsigned int>& Indices() const = 0;
		
		virtual void Scale(const glm::vec3& scale) = 0;
		virtual void RotatePitchYawRoll(float pitch, float yaw, float roll) = 0;
		virtual void RotateThetaPhiChi(float theta, float phi, float chi) = 0;
		virtual void Translate(const glm::vec3& translation) = 0;
		
		virtual const glm::mat4& GetModelMatrix() const = 0;
	};
}