#include "Model.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace d3dt
{
	std::map<std::string, ModelReference> Model::m_referencesMap;
	const std::string Model::ID_PLANE = "1";
	const std::string Model::ID_CUBE = "2";

	std::unique_ptr<ModelInstance> Model::CreatePlane(const glm::vec3& position, const glm::vec2& dimensions, Color color, std::shared_ptr<Texture> texture)
	{
		const auto halfDimensions = dimensions / 2.0f;

		std::vector<Vertex> vertices =
		{
			{ { -halfDimensions.x, -halfDimensions.y, 0.0f }, color, { 0.0f, 0.0f } },
			{ { -halfDimensions.x, halfDimensions.y, 0.0f }, color, { 0.0f, 1.0f } },
			{ { halfDimensions.x, halfDimensions.y, 0.0f }, color, { 1.0f, 1.0f } },
			{ { halfDimensions.x, -halfDimensions.y, 0.0f }, color, { 1.0f, 0.0f } },
		};

		std::vector<unsigned int> indices =
		{
			0, 1, 2,
			2, 3, 0
		};

		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };

		return CreateModel(ID_PLANE, std::move(vertices), std::move(indices), position, scale, rotation, std::move(texture));
	}
	
	std::unique_ptr<ModelInstance> Model::CreateCube(const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture)
	{
		constexpr float side = 1.0f / 2.0f;

		std::vector<Vertex> vertices(24);
		vertices[0].position = { -side,-side,-side };// 0 near side
		vertices[1].position = { side,-side,-side };// 1
		vertices[2].position = { -side,side,-side };// 2
		vertices[3].position = { side,side,-side };// 3
		vertices[4].position = { -side,-side,side };// 4 far side
		vertices[5].position = { side,-side,side };// 5
		vertices[6].position = { -side,side,side };// 6
		vertices[7].position = { side,side,side };// 7
		vertices[8].position = { -side,-side,-side };// 8 left side
		vertices[9].position = { -side,side,-side };// 9
		vertices[10].position = { -side,-side,side };// 10
		vertices[11].position = { -side,side,side };// 11
		vertices[12].position = { side,-side,-side };// 12 right side
		vertices[13].position = { side,side,-side };// 13
		vertices[14].position = { side,-side,side };// 14
		vertices[15].position = { side,side,side };// 15
		vertices[16].position = { -side,-side,-side };// 16 bottom side
		vertices[17].position = { side,-side,-side };// 17
		vertices[18].position = { -side,-side,side };// 18
		vertices[19].position = { side,-side,side };// 19
		vertices[20].position = { -side,side,-side };// 20 top side
		vertices[21].position = { side,side,-side };// 21
		vertices[22].position = { -side,side,side };// 22
		vertices[23].position = { side,side,side };// 23

		vertices[0].texCoords = { 0.0f,0.0f };
		vertices[1].texCoords = { 1.0f,0.0f };
		vertices[2].texCoords = { 0.0f,1.0f };
		vertices[3].texCoords = { 1.0f,1.0f };
		vertices[4].texCoords = { 0.0f,0.0f };
		vertices[5].texCoords = { 1.0f,0.0f };
		vertices[6].texCoords = { 0.0f,1.0f };
		vertices[7].texCoords = { 1.0f,1.0f };
		vertices[8].texCoords = { 0.0f,0.0f };
		vertices[9].texCoords = { 1.0f,0.0f };
		vertices[10].texCoords = { 0.0f,1.0f };
		vertices[11].texCoords = { 1.0f,1.0f };
		vertices[12].texCoords = { 0.0f,0.0f };
		vertices[13].texCoords = { 1.0f,0.0f };
		vertices[14].texCoords = { 0.0f,1.0f };
		vertices[15].texCoords = { 1.0f,1.0f };
		vertices[16].texCoords = { 0.0f,0.0f };
		vertices[17].texCoords = { 1.0f,0.0f };
		vertices[18].texCoords = { 0.0f,1.0f };
		vertices[19].texCoords = { 1.0f,1.0f };
		vertices[20].texCoords = { 0.0f,0.0f };
		vertices[21].texCoords = { 1.0f,0.0f };
		vertices[22].texCoords = { 0.0f,1.0f };
		vertices[23].texCoords = { 1.0f,1.0f };

		std::vector<unsigned int> indices = {
			0,2, 1,   2,3,1,
			4,5, 7,   4,7,6,
			8,10, 9,  10,11,9,
			12,13,15, 12,15,14,
			16,17,18, 18,17,19,
			20,23,21, 20,22,23
		};

		return CreateModel(ID_CUBE, std::move(vertices), std::move(indices), translation, scale, rotation, std::move(texture));
	}

	std::unique_ptr<ModelInstance> Model::CreateFromFile(const std::string& path, const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture)
	{
		Assimp::Importer importer;
		const auto modelScene = importer.ReadFile(path, aiProcess_Triangulate);
		if (!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode)
		{
			std::string str = std::string("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
			DebugBreak();
		}

		auto mesh = modelScene->mMeshes[modelScene->mRootNode->mChildren[0]->mMeshes[0]];
		std::vector<Vertex> vertices(mesh->mNumVertices);
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			vertices[i].position.x = mesh->mVertices[i].x;
			vertices[i].position.y = mesh->mVertices[i].y;
			vertices[i].position.z = mesh->mVertices[i].z;

			if (mesh->mTextureCoords[0])
			{
				vertices[i].texCoords.x = mesh->mTextureCoords[0][i].x;
				vertices[i].texCoords.y = mesh->mTextureCoords[0][i].y;
			}

			if (mesh->mNormals)
			{
				vertices[i].normals.x = mesh->mNormals[i].x;
				vertices[i].normals.y = mesh->mNormals[i].y;
				vertices[i].normals.z = mesh->mNormals[i].z;
			}
		}

		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			const auto face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		return CreateModel(path, std::move(vertices), std::move(indices), translation, scale, rotation, std::move(texture));
	}

	const std::vector<Vertex>& Model::Vertices() const
	{
		return m_vertices;
	}

	const std::vector<unsigned int>& Model::Indices() const
	{
		return m_indices;
	}

	void Model::Draw(IPipeline* pipeline, const glm::mat4& modelMatrix) const
	{
		for (const auto& mesh : m_meshes)
		{
			mesh.Draw(modelMatrix, pipeline);
		}
	}

	std::unique_ptr<ModelInstance> Model::CreateModel(const std::string& id, std::vector<Vertex> vertices, std::vector<unsigned int> indices, const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture)
	{
		if (const auto reference = m_referencesMap.find(id); reference != m_referencesMap.end())
		{
			return std::make_unique<ModelInstance>(reference->second, translation, scale, rotation, std::move(texture));
		}

		const auto newReference = ModelReference(new Model(std::move(vertices), std::move(indices)), id);
		m_referencesMap[id] = newReference;

		return std::make_unique<ModelInstance>(newReference, translation, scale, rotation, std::move(texture));
	}

	Model::Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		: m_vertices(std::move(vertices)), m_indices(std::move(indices))
	{
	}	
}