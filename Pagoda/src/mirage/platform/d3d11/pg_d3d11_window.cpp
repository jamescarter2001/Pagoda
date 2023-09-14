#include "pgpch.h"
#include "pg_d3d11_window.h"

namespace Pagoda::Mirage {
    D3D11Window::D3D11Window(const WindowProps& props) : Window(props) {
        this->m_Window = nullptr;

        Init();
    }

    Window* Window::Create(const WindowProps& props) {
        return new D3D11Window(props);
    }

    LRESULT CALLBACK D3D11Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        // sort through and find what code to run for the message given
        switch (message) {
                // this message is read when the window is closed
            case WM_DESTROY: {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            } break;
        }

        // Handle any messages the switch statement didn't
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    void D3D11Window::Init() {
        HINSTANCE hInstance = GetModuleHandle(NULL);

        // the handle for the window, filled by a function
        HWND hWnd;
        // this struct holds information for the window class
        WNDCLASSEX wc;

        // clear out the window class for use
        ZeroMemory(&wc, sizeof(WNDCLASSEX));

        // fill in the struct with the needed information
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = Pagoda::Mirage::D3D11Window::WindowProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
        wc.lpszClassName = L"WindowClass1";

        // register the window class
        RegisterClassEx(&wc);

        // create the window and use the result as the handle
        hWnd = CreateWindowEx(NULL,
                              L"WindowClass1",                // name of the window class
                              STR_TO_WSTR(this->GetTitle()).c_str(),  // title of the window
                              WS_OVERLAPPEDWINDOW,            // window style
                              300,                            // x-position of the window
                              300,                            // y-position of the window
                              this->GetWidth(),               // width of the window
                              this->GetHeight(),              // height of the window
                              NULL,                           // we have no parent window, NULL
                              NULL,                           // we aren't using menus, NULL
                              hInstance,                      // application handle
                              NULL);                          // used with multiple windows, NULL

        // display the window on the screen
        ShowWindow(hWnd, SW_SHOW);
    }

    void D3D11Window::OnUpdate() {
        MSG msg;

        // Check to see if any messages are waiting in the queue
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // translate keystroke messages into the right format
            TranslateMessage(&msg);

            // send the message to the WindowProc function
            DispatchMessage(&msg);

            // check to see if it's time to quit
            if (msg.message == WM_QUIT) {
                Base::WindowCloseEvent e = Base::WindowCloseEvent();
                this->m_WindowData.EventCallback(e);
            }
        }
    }
}