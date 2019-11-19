#include "Pipeline.h"
#include <algorithm>

namespace d3dt
{
	Pipeline::Pipeline(std::shared_ptr<Context> context)
		: m_context(std::move(context))
	{
	}
	
	void Pipeline::SetVertexShader(std::shared_ptr<VertexShader> shader)
	{
		m_vertexShader = std::move(shader);
	}

	void Pipeline::SetPixelShader(std::shared_ptr<PixelShader> shader)
	{
		m_pixelShader = std::move(shader);
	}

	void Pipeline::AddVertexBuffer(std::shared_ptr<VertexBuffer> buffer, UINT slot)
	{
		m_vertexBuffers[slot] = buffer;
	}

	void Pipeline::SetIndexBuffer(std::shared_ptr<IndexBuffer> buffer)
	{
		m_indexBuffer = std::move(buffer);
	}

	void Pipeline::SetViewport(const Viewport& viewport)
	{
		m_viewport = viewport;
	}

	void Pipeline::Prepare(const Color& renderTargetViewClearColor)
	{
		m_vertexShader->Use();
		m_pixelShader->Use();

		if (m_indexBuffer)
		{
			m_indexBuffer->Bind();
		}

		for (const auto& buffer : m_vertexBuffers)
		{
			buffer.second->Bind(buffer.first);
		}

		m_context->SetViewport(m_viewport);
		m_context->ClearRenderTargetView(renderTargetViewClearColor);
	}
	
	void Pipeline::Draw(UINT firstIndexLocation, UINT indicesCount)
	{
		m_context->Draw(firstIndexLocation, indicesCount);
	}

	void Pipeline::DrawInstanced(UINT indexCountPerInstance, UINT numberOfInstances, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanceLocation)
	{
		m_context->DrawInstanced(indexCountPerInstance, numberOfInstances, startIndexLocation, baseVertexLocation, startInstanceLocation);
	}
	
	const std::shared_ptr<Context>& Pipeline::GetContext() const
	{
		return m_context;
	}

	const std::shared_ptr<VertexShader> Pipeline::GetVertexShader() const
	{
		return m_vertexShader;
	}

	const std::shared_ptr<PixelShader> Pipeline::GetPixelShader() const
	{
		return m_pixelShader;
	}
	
	void Pipeline::Present() const
	{
		m_context->Present();
	}
}
