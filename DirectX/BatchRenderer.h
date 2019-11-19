#pragma once
#include "IPipeline.h"
#include "Models/IModelInstance.h"
#include "IConstantBuffer.h"

#include <memory>
#include <vector>
#include <map>

namespace d3dt
{
	class BatchRenderer
	{
	public:
		BatchRenderer(std::shared_ptr<IPipeline> pipeline);
		void AddModel(std::shared_ptr<IModelInstance> model);
		void SetViewProjectionMatrix(glm::mat4 matrix);
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

		class Batch
		{
		public:
			Batch() = default;

			Batch(std::shared_ptr<IModelInstance> model, const std::shared_ptr<Context>& context)
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

			void Draw(const std::shared_ptr<IPipeline>& pipeline, IConstantBuffer<ModelMatrixCBuffer>* constantBuffer, const Color& renderTargetViewClearColor) const
			{
				pipeline->AddVertexBuffer(m_vertexBuffer);
				pipeline->SetIndexBuffer(m_indexBuffer);

				pipeline->Prepare(renderTargetViewClearColor);

				ModelMatrixCBuffer modelMatrix;
				for (const auto& model : m_models)
				{
					model->Update();

					model->GetTexture()->Use();
					modelMatrix.matrix = model->GetModelMatrix();
					constantBuffer->Update(modelMatrix);

					pipeline->GetVertexShader()->SetConstantBuffer(*constantBuffer, 1);
					pipeline->Draw(0, model->Indices().size());
				}
			}

		private:
			std::string m_modelID = "";
			
			std::vector<std::shared_ptr<IModelInstance>> m_models;

			std::shared_ptr<VertexBuffer> m_vertexBuffer;
			std::shared_ptr<IndexBuffer> m_indexBuffer;
		};

		std::map<std::string, Batch> m_batches;

		std::shared_ptr<IPipeline> m_pipeline;
		
		std::unique_ptr<IConstantBuffer<ViewProjection>> m_viewProjectionConstantBuffer;
		std::unique_ptr<IConstantBuffer<ModelMatrixCBuffer>> m_modelMatrixConstantBuffer;
	};
}