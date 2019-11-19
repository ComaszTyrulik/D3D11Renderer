#pragma once
#include <string>
#include <memory>

#include "WinInclude.h"
#include "Graphics.h"

namespace d3dt
{
    class Window
    {
    public:
        Window(const std::string& name, int positionX, int positionY, int width, int height) noexcept;
        ~Window();

        Window(Window&) = delete;
        Window(Window&&) = delete;
        Window operator=(Window&) = delete;
        Window operator=(Window&&) = delete;

        int ProcessMessages();

		class WindowHandle
		{
			friend class Window;
		private:
			using HandleType = HWND;
			
			WindowHandle() = default;
			explicit WindowHandle(HandleType handle) : m_windowHandle(handle)
			{
			}
		
		public:
			~WindowHandle() = default;

			HandleType Get() const
			{
				return m_windowHandle;
			}

		private:
			HandleType m_windowHandle;
		};

		const WindowHandle Handle() const
		{
			return m_hWnd;
		}

    private:
        inline static const auto WINDOW_CLASS_NAME = "d3dWnd";
        static LRESULT CALLBACK ProcessWindowMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
        WindowHandle m_hWnd;
        inline static HINSTANCE m_hInstance = nullptr;
    };
}