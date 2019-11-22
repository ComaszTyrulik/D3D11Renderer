#pragma once
#include "IModelInstance.h"
#include "ModelReference.h"

#include <vector>
#include <memory>

namespace d3dt
{
	class ModelInstance final
		: public IModelInstance
	{
	public:
		ModelInstance(ModelReference modelRef, const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture);

		// Inherited via IGeometry
		virtual void Scale(const glm::vec3& scale) override;
		virtual void RotatePitchYawRoll(float pitch, float yaw, float roll) override;
		virtual void RotateThetaPhiChi(float theta, float phi, float chi) override;
		virtual void Translate(const glm::vec3& translation) override;

		// Inherited via IModelInstance
		virtual const std::shared_ptr<Texture>& GetTexture() const override;
		virtual void Update() override;
		virtual const glm::mat4& GetModelMatrix() const override;

		virtual const ModelReference& Reference() const override;
		virtual void SetMaterial(Material material) override;

		// Inherited via IDrawable
		virtual void Draw(IPipeline* pipeline, bool useTextures = true) const override;

	private:
		ModelReference m_modelRef;
		std::shared_ptr<Texture> m_texture;

		float m_pitch = 0.0f;
		float m_yaw = 0.0f;
		float m_roll = 0.0f;

		float m_theta = 0.0f;
		float m_phi = 0.0f;
		float m_chi = 0.0f;

		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_translation = { 0.0f, 0.0f, 0.0f };

		glm::mat4 m_modelMatrix = glm::mat4(1.0f);

		bool m_shouldUpdate = true;
	};
}