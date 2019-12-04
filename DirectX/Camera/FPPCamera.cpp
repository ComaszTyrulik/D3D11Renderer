#include "FPPCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace d3dt
{
	FPPCamera::FPPCamera(glm::vec3 position, glm::vec3 frontVector, glm::vec3 upVector, float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
		: m_FOV(fieldOfView), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane), m_farPlane(farPlane), m_position(std::move(position)), m_frontVector(std::move(frontVector)), m_upVector(std::move(upVector))
	{
		Update();
	}
	
	void FPPCamera::Update()
	{
		const auto frontVector = GetRotationMatrix() * m_frontVector;

		m_cameraMatrix =
			glm::perspectiveLH(glm::radians(45.0f), m_aspectRatio, m_nearPlane, m_farPlane) *
			glm::lookAtLH(m_position, m_position + frontVector, m_upVector);
	}

	void FPPCamera::Move(const glm::vec3& direction)
	{
		m_position += GetRotationMatrix() * direction;
	}

	void FPPCamera::Rotate(const glm::vec3& rotation)
	{
		m_rotation += glm::radians(rotation);

		if (m_rotation.y > glm::radians(360.0f) || m_rotation.y < glm::radians(-360.0f)) {
			m_rotation.y = 0.0f;
		}
		m_rotation.x = glm::clamp(m_rotation.x, 0.995f* -glm::pi<float>() / 2.0f, 0.995f * glm::pi<float>() / 2.0f);
	}
	
	const glm::mat4& FPPCamera::GetMatrix() const
	{
		return m_cameraMatrix;
	}
	
	const glm::vec3& FPPCamera::GetPosition() const
	{
		return m_position;
	}
	
	glm::mat3 FPPCamera::GetRotationMatrix()
	{
		return
			glm::rotate(glm::mat4(1.0f), m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	}
}
