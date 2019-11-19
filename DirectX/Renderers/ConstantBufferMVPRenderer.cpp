//#include "ConstantBufferMVPRenderer.h"
//#include <algorithm>
//
//namespace d3dt
//{
//	ConstantBufferMVPRenderer::ConstantBufferMVPRenderer(std::shared_ptr<IPipeline> pipeline)
//		: m_pipeline(std::move(pipeline))
//	{
//		m_viewProjectionConstantBuffer = m_pipeline->GetContext()->CreateConstantBuffer(m_viewProjectionConstantBufferData);
//	}
//
//	void ConstantBufferMVPRenderer::AddModel(std::shared_ptr<IModel> model)
//	{
//		m_models.push_back(std::move(model));
//		m_regenerateBuffers = true;
//	}
//
//	void ConstantBufferMVPRenderer::GenerateBatches()
//	{
//		if (!m_regenerateBuffers)
//		{
//			return;
//		}
//
//		std::vector<Vertex> vertices;
//		std::vector<unsigned int> indices;
//		unsigned int indicesOffset = 0;
//		UINT firstIndexLocation = 0;
//		std::size_t modelMatrixIndex = 0;
//
//		// Sort our models
//		std::sort(
//			m_models.begin(),
//			m_models.end(),
//			[](const std::shared_ptr<IModel>& m1, const std::shared_ptr<IModel>& m2) -> auto
//			{
//				return m1->GetTexture() < m2->GetTexture();
//			}
//		);
//
//		std::shared_ptr<Texture> previousTexture = nullptr;
//		for (auto& model : m_models)
//		{
//			model->Update();
//
//			auto l_vertices = model->Vertices();
//			auto l_indices = model->Indices();
//
//			// Change indices offset
//			std::transform(
//				l_indices.begin(),
//				l_indices.end(),
//				l_indices.begin(),
//				[&indicesOffset](unsigned int element) -> auto { return element + indicesOffset; }
//			);
//
//			vertices.insert(vertices.end(), l_vertices.begin(), l_vertices.end());
//			indices.insert(indices.end(), l_indices.begin(), l_indices.end());
//
//			if (previousTexture == nullptr || model->GetTexture() != previousTexture)
//			{
//				modelMatrixIndex = 0;
//
//				previousTexture = model->GetTexture();
//				m_batches.emplace_back(firstIndexLocation, previousTexture, m_pipeline->GetContext());
//				m_batches.back().Add(static_cast<UINT>(l_indices.size()));
//				m_batches.back().SetMatrix(modelMatrixIndex++, &model->GetModelMatrix());
//			}
//			else
//			{
//				m_batches.back().Add(static_cast<UINT>(l_indices.size()));
//				m_batches.back().SetMatrix(modelMatrixIndex++, &model->GetModelMatrix());
//			}
//
//			indicesOffset += static_cast<unsigned int>(l_vertices.size());
//			firstIndexLocation += static_cast<UINT>(l_indices.size());
//		}
//
//		const auto& context = m_pipeline->GetContext();
//		m_pipeline->AddVertexBuffer(context->CreateVertexBuffer(vertices));
//		m_pipeline->SetIndexBuffer(context->CreateIndexBuffer(indices));
//
//		m_regenerateBuffers = false;
//	}
//
//	void ConstantBufferMVPRenderer::SetViewProjectionMatrix(glm::mat4 matrix)
//	{
//		m_viewProjectionConstantBufferData.vp = std::move(matrix);
//		m_viewProjectionConstantBuffer->Update(m_viewProjectionConstantBufferData);
//	}
//
//	void ConstantBufferMVPRenderer::Render(const Color& renderTargetViewClearColor)
//	{
//		m_pipeline->Prepare(renderTargetViewClearColor);
//		m_pipeline->GetVertexShader()->SetConstantBuffer(*m_viewProjectionConstantBuffer);
//		for (auto& batch : m_batches)
//		{
//			batch.Update();
//			batch.Draw(m_pipeline);
//		}
//		m_pipeline->Present();
//	}
//}
