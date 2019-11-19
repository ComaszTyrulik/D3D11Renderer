#include "InstancedRenderer.h"
#include <algorithm>

namespace d3dt
{
	InstancedRenderer::InstancedRenderer(std::shared_ptr<IPipeline> pipeline)
		: m_pipeline(std::move(pipeline))
	{
		m_viewProjectionConstantBuffer = m_pipeline->GetContext()->CreateConstantBuffer(m_viewProjectionConstantBufferData);
	}
	
	void InstancedRenderer::AddModel(std::shared_ptr<IModelInstance> model)
	{
		const auto modelId = model->Reference().ID();

		auto mIt = m_batches.find(modelId);
		if (mIt == m_batches.end())
		{
			m_batches[modelId] = Batch2(model, m_pipeline->GetContext());

			return;
		}

		mIt->second.Add(std::move(model));
	}

	void InstancedRenderer::SetViewProjectionMatrix(glm::mat4 matrix)
	{
		m_viewProjectionConstantBufferData.vp = std::move(matrix);
		m_viewProjectionConstantBuffer->Update(m_viewProjectionConstantBufferData);
	}

	void InstancedRenderer::Prepare()
	{
		for (auto& batch : m_batches)
		{
			batch.second.Update(m_pipeline->GetContext());
		}
	}
	
	void InstancedRenderer::Render(const Color& renderTargetViewClearColor)
	{
		m_pipeline->GetVertexShader()->SetConstantBuffer(*m_viewProjectionConstantBuffer);
		for (const auto& batch : m_batches)
		{
			batch.second.Draw(m_pipeline, renderTargetViewClearColor);
		}
		m_pipeline->Present();
	}
}
