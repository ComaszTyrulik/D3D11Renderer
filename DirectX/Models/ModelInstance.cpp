#include "ModelInstance.h"

#include <glm/glm.hpp>
#include <glm\gtx\transform.hpp>

namespace d3dt
{
	ModelInstance::ModelInstance(ModelReference modelRef, const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture)
		: m_modelRef(std::move(modelRef)), m_texture(std::move(texture))
	{
		Translate(translation);
		Scale(scale);
		RotatePitchYawRoll(rotation.x, rotation.y, rotation.z);
	}

	void ModelInstance::Scale(const glm::vec3& scale)
	{
		m_scale += scale;
		m_shouldUpdate = true;
	}

	void ModelInstance::RotatePitchYawRoll(float pitch, float yaw, float roll)
	{
		m_pitch += pitch;
		m_yaw += yaw;
		m_roll += roll;

		if (m_pitch > 360.0f) { m_pitch = 0.0f; }
		if (m_yaw > 360.0f) { m_yaw = 0.0f; }
		if (m_roll > 360.0f) { m_roll = 0.0f; }

		m_shouldUpdate = true;
	}

	void ModelInstance::RotateThetaPhiChi(float theta, float phi, float chi)
	{
		m_theta += theta;
		m_phi += phi;
		m_chi += chi;

		if (m_theta > 360.0f) { m_theta = 0.0f; }
		if (m_phi > 360.0f) { m_phi = 0.0f; }
		if (m_chi > 360.0f) { m_chi = 0.0f; }

		m_shouldUpdate = true;
	}

	void ModelInstance::Translate(const glm::vec3& translation)
	{
		m_translation += translation;
		m_shouldUpdate = true;
	}

	const std::shared_ptr<Texture>& ModelInstance::GetTexture() const
	{
		return m_texture;
	}

	void ModelInstance::Update()
	{
		if (!m_shouldUpdate)
		{
			return;
		}

		m_modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_theta), glm::vec3(1.0f, 0.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_phi), glm::vec3(0.0f, 1.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_chi), glm::vec3(0.0f, 0.0f, 1.0f));

		m_modelMatrix = glm::translate(m_modelMatrix, m_translation);

		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_roll), glm::vec3(0.0f, 0.0f, 1.0f));

		m_modelMatrix = glm::scale(m_modelMatrix, m_scale);

		m_shouldUpdate = false;
	}

	const glm::mat4& ModelInstance::GetModelMatrix() const
	{
		return m_modelMatrix;
	}
	
	const ModelReference& ModelInstance::Reference() const
	{
		return m_modelRef;
	}

	void ModelInstance::SetMaterial(Material material)
	{
		m_modelRef->SetMaterial(std::move(material));
	}
	
	void ModelInstance::Draw(IPipeline* pipeline, bool useTextures) const
	{
		m_modelRef->Draw(pipeline, m_modelMatrix, useTextures);
	}
}