#pragma once
#include "../IPipeline.h"
#include "../Models/IModelInstance.h"
#include "../IConstantBuffer.h"

#include <memory>
#include <vector>
#include <map>

namespace d3dt
{
	class InstancedRenderer
	{
	public:
		InstancedRenderer(std::shared_ptr<IPipeline> pipeline);
		void AddModel(std::shared_ptr<IModelInstance> model);
		void SetViewProjectionMatrix(glm::mat4 matrix);
		void Prepare();
		void Render(const Color& renderTargetViewClearColor);
	
	private:
		struct ViewProjection
		{
			glm::mat4 vp = glm::mat4(1.0f);
		} m_viewProjectionConstantBufferData = ViewProjection();

		struct ModelMatrixCBuffer
		{
			glm::mat4 matrix = glm::mat4(1.0f);
		} m_modelMatrixCBufferData;

		class Batch2
		{
		public:
			Batch2() = default;

			Batch2(std::shared_ptr<IModelInstance> model, const std::shared_ptr<Context>& context)
			{
				m_modelID = model->Reference().ID();
				m_vertexBuffer = context->CreateVertexBuffer(model->Vertices());
				m_indexBuffer = context->CreateIndexBuffer(model->Indices());

				m_models.push_back(std::move(model));
			}

			void Add(std::shared_ptr<IModelInstance> model)
			{
				if (model->Reference().ID() != m_modelID)
				{
					DebugBreak();

					return;
				}

				m_models.push_back(std::move(model));
			}

			void Update(const std::shared_ptr<Context>& context)
			{
				std::vector<glm::mat4> matrices(m_models.size());
				for (std::size_t i = 0; i < m_models.size(); ++i)
				{
					matrices[i] = m_models[i]->GetModelMatrix();
				}

				if (m_instanceDataBuffer == nullptr)
				{
					m_instanceDataBuffer = context->CreateVertexBuffer(matrices);
					
					return;
				}

				m_instanceDataBuffer->Update(matrices);
			}

			void Draw(const std::shared_ptr<IPipeline>& pipeline, const Color& renderTargetViewClearColor) const
			{
				pipeline->AddVertexBuffer(m_vertexBuffer, 0);
				pipeline->AddVertexBuffer(m_instanceDataBuffer, 1);

				pipeline->SetIndexBuffer(m_indexBuffer);
				pipeline->Prepare(renderTargetViewClearColor);

				const auto& model = m_models.front();
				
				model->GetTexture()->Use();
				pipeline->DrawInstanced(model->Indices().size(), m_models.size(), 0, 0, 0);
			}

		private:
			std::string m_modelID = "";
			
			std::vector<std::shared_ptr<IModelInstance>> m_models;

			std::shared_ptr<VertexBuffer> m_instanceDataBuffer;

			std::shared_ptr<VertexBuffer> m_vertexBuffer;
			std::shared_ptr<IndexBuffer> m_indexBuffer;
		};

		std::map<std::string, Batch2> m_batches;
		std::shared_ptr<IPipeline> m_pipeline;
		std::unique_ptr<IConstantBuffer<ViewProjection>> m_viewProjectionConstantBuffer;
	};
}