//#pragma once
//#include <memory>
//#include "../IPipeline.h"
//#include "../Batch.h"
//
//namespace d3dt
//{
//	class ConstantBufferMVPRenderer
//	{
//	/*
//		We are going to need a VP matrix constant buffer the same as we have now.
//		Additionally we're going to have a one big constant buffer containing model matrix for all our models
//		let's same a struct like this:
//		struct ModelMatrices
//		{
//			glm::mat4[1000]
//		}
//
//		What we're going to need:
//		1. Every batch we'll have to also store those matrices
//		2. Vertex and Index buffers will be created and filled only once
//	*/
//	public:
//		ConstantBufferMVPRenderer(std::shared_ptr<IPipeline> pipeline);
//		void AddModel(std::shared_ptr<IModel> model);
//		void GenerateBatches();
//		void SetViewProjectionMatrix(glm::mat4 matrix);
//		void Render(const Color& renderTargetViewClearColor);
//	private:
//		bool m_regenerateBuffers = true;
//		static const std::size_t MAX_NUMBER_OF_MATRICES = 1000;
//
//		struct ViewProjection
//		{
//			glm::mat4 vp = glm::mat4(1.0f);
//		} m_viewProjectionConstantBufferData = ViewProjection();
//
//		class ModelMatrixBatch : public Batch
//		{
//		public:
//			ModelMatrixBatch(UINT firstIndexLocation, std::shared_ptr<Texture> texture, const std::shared_ptr<Context>& context)
//				: Batch(firstIndexLocation, texture)
//			{
//				m_constantBuffer = context->CreateConstantBuffer(m_matrices);
//				m_numberOfMatrices = 1;
//			}
//
//			bool CanFit() const
//			{
//				return m_numberOfMatrices == MAX_NUMBER_OF_MATRICES;
//			}
//
//			void Add(UINT indexCount)
//			{
//				m_numberOfMatrices++;
//				Batch::Add(indexCount);
//			}
//
//			void SetMatrix(std::size_t index, const glm::mat4* matrix)
//			{
//				m_matricesPtrs[index] = matrix;
//			}
//
//			void Update()
//			{
//				for (std::size_t i = 0; i < MAX_NUMBER_OF_MATRICES; ++i)
//				{
//					m_matrices.matrices[i] = *m_matricesPtrs[i];
//				}
//			}
//
//			void Draw(const std::shared_ptr<IPipeline>& pipeline) const
//			{
//				m_constantBuffer->Update(m_matrices);
//				pipeline->GetVertexShader()->SetConstantBuffer(*m_constantBuffer, 1);
//
//				Batch::Draw(pipeline);
//			}
//
//		private:
//			const glm::mat4* m_matricesPtrs[MAX_NUMBER_OF_MATRICES];
//			std::size_t m_numberOfMatrices = 0;
//
//			struct ModelMatrices
//			{
//				glm::mat4 matrices[MAX_NUMBER_OF_MATRICES];
//			} m_matrices;
//			std::unique_ptr<IConstantBuffer<ModelMatrices>> m_constantBuffer;
//		};
//
//		std::shared_ptr<IPipeline> m_pipeline;
//		std::vector<std::shared_ptr<IModel>> m_models;
//		std::vector<ModelMatrixBatch> m_batches;
//		std::unique_ptr<IConstantBuffer<ViewProjection>> m_viewProjectionConstantBuffer;
//	};
//};