#pragma once
#include <string>
#include <memory>

#include "WinInclude.h"
#include "Graphics.h"
#include <unordered_map>
#include "vendor/Chili/Keyboard.h"

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
			explicit WindowHandle(HandleType handle, int width, int height) : m_windowHandle(handle), m_width(width), m_height(height)
			{
			}
		
		public:
			~WindowHandle() = default;

			HandleType Get() const
			{
				return m_windowHandle;
			}

			int Width() const
			{
				return m_width;
			}

			int Height() const
			{
				return m_height;
			}

		private:
			HandleType m_windowHandle;
			int m_width, m_height;
		};

		const WindowHandle Handle() const
		{
			return m_hWnd;
		}

	public:
		static Keyboard kbd;

	private:
		static void ProcessKeyboard(WPARAM wParam, bool isPressed);

    private:
        inline static const auto WINDOW_CLASS_NAME = "d3dWnd";
        static LRESULT CALLBACK ProcessWindowMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
        WindowHandle m_hWnd;
        inline static HINSTANCE m_hInstance = nullptr;

		static std::unordered_map<int, bool> m_keys;
    };
}