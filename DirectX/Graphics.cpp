#include "Graphics.h"

namespace d3dt
{
    Graphics::Graphics(HWND hwnd)
    {
        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = hwnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D10_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &m_swapChain,
            &m_device,
            nullptr,
            &m_context
        );

        wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
        m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
        m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
    }
    
    void Graphics::ClearRenderTarger(float red, float green, float blue, float alpha)
    {
        const float color[] = { red, green, blue, alpha };
        m_context->ClearRenderTargetView(m_renderTargetView.Get(), color);
    }

    void Graphics::EndFrame()
    {
        m_swapChain->Present(1u, 0u);
    }
}