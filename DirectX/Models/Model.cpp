#include "Model.h"

namespace d3dt
{
	std::map<std::string, ModelReference> Model::m_referencesMap;

	std::unique_ptr<ModelInstance> Model::CreateFromFile(
		const std::string& path,
		std::shared_ptr<Context> context,
		std::shared_ptr<Texture> texture,
		const glm::vec3& translation,
		const glm::vec3& scale,
		const glm::vec3& rotation,
		const Color& color
	)
	{
		if (const auto model = FindModel(path); model.has_value())
		{
			return std::make_unique<ModelInstance>(model.value(), translation, scale, rotation, std::move(texture));
		}

		Assimp::Importer importer;
		const auto modelScene = importer.ReadFile(path, aiProcess_Triangulate);
		if (!modelScene || modelScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !modelScene->mRootNode)
		{
			std::string str = std::string("ERROR::ASSIMP::" + std::string(importer.GetErrorString()));
			DebugBreak();
		}

		return CreateModel(path, modelScene, std::move(context), translation, scale, rotation, std::move(texture), color);
	}

	void Model::Draw(IPipeline* pipeline, const glm::mat4& modelMatrix, bool useTextures) const
	{
		for (const auto& mesh : m_meshes)
		{
			mesh.Draw(modelMatrix, pipeline, useTextures);
		}
	}

	void Model::SetMaterial(Material material)
	{
		for (auto& mesh : m_meshes)
		{
			mesh.SetMaterial(std::move(material));
		}
	}

	std::optional<ModelReference> Model::FindModel(const std::string& id)
	{
		if (const auto reference = m_referencesMap.find(id); reference != m_referencesMap.end())
		{
			return reference->second;
		}

		return {};
	}

	std::unique_ptr<ModelInstance> Model::CreateModel(const std::string& id, const aiScene* scene, std::shared_ptr<Context> context, const glm::vec3& translation, const glm::vec3& scale, const glm::vec3& rotation, std::shared_ptr<Texture> texture, const Color& color)
	{
		auto newModel = new Model(std::move(context), color, std::move(texture));
		newModel->ProcessNode(scene->mRootNode, scene);

		const auto newReference = ModelReference(newModel, id);
		m_referencesMap[id] = newReference;

		return std::make_unique<ModelInstance>(newReference, translation, scale, rotation, std::move(texture));
	}
	
	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_meshes.push_back(ProcessMesh(mesh, scene));
		}
	
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	
	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
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

			vertices[i].color = m_color;
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

		std::shared_ptr<TexturesArray> diffuseTextures = nullptr;
		std::shared_ptr<TexturesArray> specularTextures = nullptr;
		if (mesh->mMaterialIndex >= 0)
		{
			const auto material = scene->mMaterials[mesh->mMaterialIndex];
			diffuseTextures = LoadTexturesByType(aiTextureType::aiTextureType_DIFFUSE, material);
			specularTextures = LoadTexturesByType(aiTextureType::aiTextureType_SPECULAR, material);
		}

		return Mesh(std::move(vertices), std::move(indices), m_context, std::move(diffuseTextures), std::move(specularTextures));
	}
	
	std::shared_ptr<TexturesArray> Model::LoadTexturesByType(aiTextureType type, aiMaterial* material)
	{
		std::vector<beu::Image> images;
		for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
		{
			aiString str;
			material->GetTexture(type, i, &str);
			std::string texturesFolder = "ModelsFiles/nanosuit/";
			texturesFolder += str.C_Str();
			images.push_back(
				beu::ImageLoader::loadImage(
					texturesFolder,
					beu::PixelsFormat::BE_RGBA
				)
			);
		}

		return m_context->CreateTexturesArray(images);
	}
}