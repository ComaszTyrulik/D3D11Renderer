#pragma once
#include "WinInclude.h"
#include <d3d11.h>

namespace d3dt
{
	class Viewport
	{
		friend class Context;
	public:
		Viewport(float width, float height)
		{
			m_d3dViewport.Width = width;
			m_d3dViewport.Height = height;
			m_d3dViewport.MinDepth = 0.0f;
			m_d3dViewport.MaxDepth = 1.0f;
			m_d3dViewport.TopLeftX = 0.0f;
			m_d3dViewport.TopLeftY = 0.0f;
		}

	private:
		D3D11_VIEWPORT m_d3dViewport;
	};
}