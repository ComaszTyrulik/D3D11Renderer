#include "BatchRenderer.h"
#include <algorithm>

namespace d3dt
{
	BatchRenderer::BatchRenderer(std::shared_ptr<IPipeline> pipeline)
		: m_pipeline(std::move(pipeline))
	{
		m_viewProjectionConstantBuffer = m_pipeline->GetContext()->CreateConstantBuffer(m_viewProjectionConstantBufferData);
		m_modelMatrixConstantBuffer = m_pipeline->GetContext()->CreateConstantBuffer(m_modelMatrixCBufferData);
	}
	
	void BatchRenderer::AddModel(std::shared_ptr<IModelInstance> model)
	{
		const auto modelId = model->Reference().ID();

		auto mIt = m_batches.find(modelId);
		if (mIt == m_batches.end())
		{
			m_batches[modelId] = Batch(std::move(model), m_pipeline->GetContext());

			return;
		}

		mIt->second.Add(std::move(model));
	}

	void BatchRenderer::SetViewProjectionMatrix(glm::mat4 matrix)
	{
		m_viewProjectionConstantBufferData.vp = std::move(matrix);
		m_viewProjectionConstantBuffer->Update(m_viewProjectionConstantBufferData);
	}
	
	void BatchRenderer::Render(const Color& renderTargetViewClearColor)
	{
		m_pipeline->GetVertexShader()->SetConstantBuffer(*m_viewProjectionConstantBuffer);
		for (const auto& batch : m_batches)
		{
			batch.second.Draw(m_pipeline, m_modelMatrixConstantBuffer.get(), renderTargetViewClearColor);
		}
		m_pipeline->Present();
	}
}
