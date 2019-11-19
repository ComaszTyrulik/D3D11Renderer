#pragma once
#include <vector>
#include <memory>

#include "Context.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Viewport.h"

namespace d3dt
{
	class IPipeline
	{
	public:
		virtual ~IPipeline() = default;
		
		virtual void SetVertexShader(std::shared_ptr<VertexShader> shader) = 0;
		virtual void SetPixelShader(std::shared_ptr<PixelShader> shader) = 0;
		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> buffer, UINT slot = 0) = 0;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> buffer) = 0;
		virtual void SetViewport(const Viewport& viewport) = 0;
		virtual void Prepare(const Color& renderTargetViewClearColor) = 0;
		virtual void Draw(UINT firstIndexLocation, UINT indicesCount) = 0;
		virtual void DrawInstanced(UINT indexCountPerInstance, UINT numberOfInstances, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanceLocation) = 0;
		virtual void Present() const = 0;
		
		virtual const std::shared_ptr<Context>& GetContext() const = 0;
		virtual const std::shared_ptr<VertexShader> GetVertexShader() const = 0;
		virtual const std::shared_ptr<PixelShader> GetPixelShader() const = 0;
	};
}