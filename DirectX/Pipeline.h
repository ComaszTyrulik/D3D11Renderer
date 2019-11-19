#pragma once
#include <vector>
#include <memory>
#include <map>

#include "IPipeline.h"
#include "Context.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Viewport.h"

namespace d3dt
{
	class Pipeline
		: public IPipeline
	{
	public:
		Pipeline(std::shared_ptr<Context> context);

		virtual void SetVertexShader(std::shared_ptr<VertexShader> shader) override;
		virtual void SetPixelShader(std::shared_ptr<PixelShader> shader) override;
		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> buffer, UINT slot = 0) override;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> buffer) override;
		virtual void SetViewport(const Viewport& viewport) override;
		virtual void Prepare(const Color& renderTargetViewClearColor) override;
		virtual void Draw(UINT firstIndexLocation, UINT indicesCount) override;
		virtual void DrawInstanced(UINT indexCountPerInstance, UINT numberOfInstances, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanceLocation) override;
		virtual void Present() const override;
		
		virtual const std::shared_ptr<Context>& GetContext() const override;
		virtual const std::shared_ptr<VertexShader> GetVertexShader() const;
		virtual const std::shared_ptr<PixelShader> GetPixelShader() const;

	private:
		Viewport m_viewport = Viewport(800.0f, 600.0f);

		std::map<UINT, std::shared_ptr<VertexBuffer>> m_vertexBuffers;

		std::shared_ptr<Context> m_context;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		std::shared_ptr<VertexShader> m_vertexShader;
		std::shared_ptr<PixelShader> m_pixelShader;
	};
}