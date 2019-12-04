#pragma once
#include "ICamera.h"

namespace d3dt
{
	class FPPCamera
		: public ICamera
	{
	private:
		static constexpr float DEFAULT_ASPECT_RATIO = 800.0f / 600.0f;

	public:
		FPPCamera(
			glm::vec3 position,
			glm::vec3 frontVector,
			glm::vec3 upVector,
			float fieldOfView = 45.0f,
			float aspectRatio = DEFAULT_ASPECT_RATIO,
			float nearPlane = 0.1f,
			float farPlane = 500.0f
		);

		// Inherited via ICamera
		virtual void Update() override;

		virtual void Move(const glm::vec3& direction) override;
		virtual void Rotate(const glm::vec3& rotation) override;

		virtual const glm::mat4& GetMatrix() const override;
		virtual const glm::vec3& GetPosition() const override;

	private:
		glm::mat3 GetRotationMatrix();

	private:
		float m_FOV;
		float m_aspectRatio;
		float m_nearPlane;
		float m_farPlane;

		glm::vec3 m_position;
		glm::vec3 m_frontVector;
		glm::vec3 m_upVector;
		glm::vec3 m_rotation = glm::vec3(0.0f);

		glm::mat4 m_cameraMatrix = glm::mat4(1.0f);
	};
}