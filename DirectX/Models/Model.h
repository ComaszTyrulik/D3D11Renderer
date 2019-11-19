#pragma once
#include "IModel.h"
#include "ModelReference.h"
#include "ModelInstance.h"
#include "IDrawable.h"

#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../IConstantBuffer.h"
#include "../Context.h"
#include "../IPipeline.h"

#include <map>
#include <glm/glm.hpp>

namespace d3dt
{
	class Mesh
	{
		friend class Model;

	private:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::shared_ptr<Context> context)
			: m_context(std::move(context)), m_vertices(std::move(vertices)), m_indices(std::move(indices))
		{
			m_vertexBuffer = m_context->CreateVertexBuffer(m_vertices);
			m_indexBuffer = m_context->CreateIndexBuffer(m_indices);

			m_worldMatrixCBuffer = m_context->CreateConstantBuffer(WorldMatrixCBuffer());
		}

		void Draw(const glm::mat4& worldMatrix, IPipeline* pipeline) const
		{
			WorldMatrixCBuffer bufferData{ worldMatrix };
			m_worldMatrixCBuffer->Update(bufferData);
			pipeline->GetVertexShader()->SetConstantBuffer(*m_worldMatrixCBuffer);

			m_vertexBuffer->Bind();
			m_indexBuffer->Bind();

			pipeline->Draw(0, m_indices.size());
		}

	private:
		struct WorldMatrixCBuffer
		{
			glm::mat4 data = glm::mat4(1.0f);
		};

	private:
		std::shared_ptr<Context> m_context;
		std::unique_ptr<IConstantBuffer<WorldMatrixCBuffer>> m_worldMatrixCBuffer;
		
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
	};


	class Model final
		: public IModel
	{
	public:
		static std::unique_ptr<ModelInstance> CreatePlane(const glm::vec3& position, const glm::vec2& dimensions, Color color, std::shared_ptr<Texture> texture);
		static std::unique_ptr<ModelInstance> CreateCube(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture);
		static std::unique_ptr<ModelInstance> CreateFromFile(const std::string& path, const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture);

		// Inherited via IModel
		virtual const std::vector<Vertex>& Vertices() const override;
		virtual const std::vector<unsigned int>& Indices() const override;

		virtual void Draw(IPipeline* pipeline, const glm::mat4& modelMatrix) const override;

	private:
		static std::unique_ptr<ModelInstance> CreateModel(const std::string& id, std::vector<Vertex> vertices, std::vector<unsigned int> indices, const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture);

		Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	
	private:
		static const std::string ID_PLANE;
		static const std::string ID_CUBE;

		static std::map<std::string, ModelReference> m_referencesMap;

	private:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		std::vector<Mesh> m_meshes;
	};
}