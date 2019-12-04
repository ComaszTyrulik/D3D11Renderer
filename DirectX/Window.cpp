#include "Window.h"

namespace d3dt
{
	Keyboard Window::kbd = Keyboard();

	std::unordered_map<int, bool> Window::m_keys = std::unordered_map<int, bool>();
    Window::Window(const std::string & name, int positionX, int positionY, int width, int height) noexcept
    {
        Window::m_hInstance = GetModuleHandle(nullptr);
        WNDCLASSEX windowClass = { 0 };
        windowClass.cbSize = sizeof(windowClass);
        windowClass.style = CS_OWNDC;
        windowClass.lpfnWndProc = Window::ProcessWindowMessage;
        windowClass.cbClsExtra = 0;
        windowClass.cbWndExtra = 0;
        windowClass.hInstance = Window::m_hInstance;
        windowClass.hIcon = nullptr;
        windowClass.hCursor = nullptr;
        windowClass.hbrBackground = nullptr;
        windowClass.lpszMenuName = nullptr;
        windowClass.lpszClassName = Window::WINDOW_CLASS_NAME;
        windowClass.hIconSm = nullptr;
        RegisterClassEx(&windowClass);

		m_hWnd = WindowHandle(
			CreateWindowEx(
				0, Window::WINDOW_CLASS_NAME, name.c_str(),
				WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
				positionX, positionY, width, height,
				nullptr, nullptr, Window::m_hInstance, nullptr
			),
			width,
			height
		);

        ShowWindow(m_hWnd.m_windowHandle, SW_SHOW);
    }

    Window::~Window()
    {
        DestroyWindow(m_hWnd.m_windowHandle);
    }

    int Window::ProcessMessages()
    {
        MSG msg;
        // while queue has messages, remove and dispatch them (but do not block on empty queue)
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            // check for quit because peekmessage does not signal this via return val
            if (msg.message == WM_QUIT) {
                // return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
                return static_cast<int>(msg.wParam);
            }

            // TranslateMessage will post auxilliary WM_CHAR messages from key msgs
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // return empty optional when not quitting app
        return 0;
    }

	LRESULT CALLBACK Window::ProcessWindowMessage(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message) {
            case WM_CLOSE:
                PostQuitMessage(1);
                return 0;
                break;
            case WM_KEYDOWN:
                if (wParam == VK_ESCAPE) {
                    PostQuitMessage(1);
                    
                    return 0;
                }
			case WM_SYSKEYDOWN:
				if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // filter autorepeat
				{
					kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
				}
				break;
			case WM_KEYUP:
			case WM_SYSKEYUP:
				kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
				break;
			case WM_CHAR:
				kbd.OnChar(static_cast<unsigned char>(wParam));
				break;
        }
        
        return DefWindowProc(windowHandle, message, wParam, lParam);
    }
}