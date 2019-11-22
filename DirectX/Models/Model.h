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
#include "../Material.h"

#include <map>
#include <glm/glm.hpp>
#include <optional>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace d3dt
{
	class Mesh
	{
		friend class Model;

	private:
		Mesh(
				std::vector<Vertex> vertices,
				std::vector<unsigned int> indices,
				std::shared_ptr<Context> context,
				std::shared_ptr<TexturesArray> diffuseTextures,
				std::shared_ptr<TexturesArray> specularTextures
			)
			:
			m_context(std::move(context)),
			m_vertices(std::move(vertices)),
			m_indices(std::move(indices)),
			m_diffuseTextures(std::move(diffuseTextures)),
			m_specularTextures(std::move(specularTextures))
		{
			m_vertexBuffer = m_context->CreateVertexBuffer(m_vertices);
			m_indexBuffer = m_context->CreateIndexBuffer(m_indices);

			if (m_diffuseTextures)
			{
				m_numberOfTextures.diffuseTexturesCount = m_diffuseTextures->Count();
			}

			if (m_specularTextures)
			{
				m_numberOfTextures.specularTexturesCount = m_specularTextures->Count();
			}

			m_worldMatrixCBuffer = m_context->CreateConstantBuffer(WorldMatrixCBuffer());
			m_texturesCountCBuffer = m_context->CreateConstantBuffer(m_numberOfTextures);

			m_materialCBuffer = m_context->CreateConstantBuffer(m_materialCBufferData);
		}

		void Draw(const glm::mat4& worldMatrix, IPipeline* pipeline, bool useTextures) const
		{
			WorldMatrixCBuffer bufferData{ worldMatrix };
			m_worldMatrixCBuffer->Update(bufferData);

			pipeline->GetVertexShader()->SetConstantBuffer(*m_worldMatrixCBuffer, 1);

			m_vertexBuffer->Bind();
			m_indexBuffer->Bind();

			if (useTextures)
			{
				if (m_diffuseTextures)
				{
					m_diffuseTextures->Use(0);
				}

				if (m_specularTextures)
				{
					m_specularTextures->Use(1);
				}

				pipeline->GetPixelShader()->SetConstantBuffer(*m_texturesCountCBuffer, 0);
			}

			pipeline->GetPixelShader()->SetConstantBuffer(*m_materialCBuffer, 3);
			pipeline->Draw(0, m_indices.size());
		}

		void SetMaterial(Material material)
		{
			m_materialCBufferData.data = std::move(material);
			m_materialCBuffer->Update(m_materialCBufferData);
		}

	private:
		struct WorldMatrixCBuffer
		{
			glm::mat4 data = glm::mat4(1.0f);
		};

		struct NumberOfTextures
		{
			int diffuseTexturesCount = 0;
			int specularTexturesCount = 0;
		} m_numberOfTextures;

		struct MaterialCBufferData
		{
			Material data;
		} m_materialCBufferData;

	private:
		std::shared_ptr<Context> m_context;
		std::unique_ptr<IConstantBuffer<WorldMatrixCBuffer>> m_worldMatrixCBuffer;
		std::unique_ptr<IConstantBuffer<NumberOfTextures>> m_texturesCountCBuffer;
		std::unique_ptr<IConstantBuffer<MaterialCBufferData>> m_materialCBuffer;
		
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

		std::shared_ptr<TexturesArray> m_diffuseTextures;
		std::shared_ptr<TexturesArray> m_specularTextures;
	};

	class Model final
		: public IModel
	{
	public:
		static std::unique_ptr<ModelInstance> CreateFromFile(
			const std::string& path,
			std::shared_ptr<Context> context,
			std::shared_ptr<Texture> texture,
			const glm::vec3& translation = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f),
			const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f),
			const Color& color = Color{ 1.0f, 1.0f, 1.0f, 1.0f }
		);

		// Inherited via IModel
		virtual void Draw(IPipeline* pipeline, const glm::mat4& modelMatrix, bool useTextures) const override;
		virtual void SetMaterial(Material material) override;

	private:
		static std::optional<ModelReference> FindModel(const std::string& id);
		static std::unique_ptr<ModelInstance> CreateModel(
			const std::string& id,
			const aiScene* scene,
			std::shared_ptr<Context> context,
			const glm::vec3& translation,
			const glm::vec3& scale,
			const glm::vec3& rotation,
			std::shared_ptr<Texture> texture,
			const Color& color
		);
		
		Model(std::shared_ptr<Context> context, const Color& color, std::shared_ptr<Texture> texture)
			: m_context(std::move(context)), m_color(color), m_texture(std::move(texture))
		{};

		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		std::shared_ptr<TexturesArray> LoadTexturesByType(aiTextureType type, aiMaterial* material);
	
	private:
		static std::map<std::string, ModelReference> m_referencesMap;

	private:
		Color m_color;

		std::shared_ptr<Context> m_context;
		std::vector<Mesh> m_meshes;
		std::shared_ptr<Texture> m_texture;
	};
}