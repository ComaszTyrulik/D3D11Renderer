#pragma once

#include "WinInclude.h"
#include <d3d11.h>
#include <wrl.h>

namespace d3dt
{
    namespace wrl = Microsoft::WRL;

    class Graphics
    {
    public:
        Graphics(HWND hwnd);
        ~Graphics() = default;

        void ClearRenderTarger(float red, float green, float blue, float alpha = 1.0f);
        void EndFrame();

    private:
        wrl::ComPtr<ID3D11Device> m_device;
        wrl::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
        wrl::ComPtr<ID3D11DeviceContext> m_context = nullptr;
        wrl::ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
    };
}