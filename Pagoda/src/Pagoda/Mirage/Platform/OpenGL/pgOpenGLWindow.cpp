#include "pgpch.h"

#include "pgOpenGLWindow.h"

namespace Pagoda::Mirage {

    OpenGLWindow::OpenGLWindow(const WindowProps& props) : Window(props) {
        this->m_Window = nullptr;

        /* #ifdef PG_PLATFORM_MACOS
        this->m_GLVersion = GLVersion(3, 2, GLFW_OPENGL_CORE_PROFILE);
        #endif*/

        Init();
    }

    OpenGLWindow::~OpenGLWindow() {
        glfwDestroyWindow(m_Window);
    }

    Window* Window::Create(const WindowProps& props) {
        return new OpenGLWindow(props);
    }

    void OpenGLWindow::Init() {
        PG_CORE_TRACE("Creating new OpenGL window: {0} ({1}, {2})", this->m_WindowData.Title, this->m_WindowData.Width, this->m_WindowData.Height);
        PG_CORE_ASSERT(glfwInit(), "GLFW initialization error.");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_GLVersion.Major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_GLVersion.Minor);
        glfwWindowHint(GLFW_OPENGL_PROFILE, m_GLVersion.Profile);

        /* Create a windowed mode window and its OpenGL context */
        m_Window = glfwCreateWindow(this->m_WindowData.Width, this->m_WindowData.Height, this->m_WindowData.Title.c_str(), NULL, NULL);
        PG_CORE_ASSERT(this->m_Window, "Unable to create OpenGL window context.");

        // Map window data structure to GL internal pointer.
        glfwSetWindowUserPointer(this->m_Window, &this->m_WindowData);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_Window);

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            Base::WindowCloseEvent e = Base::WindowCloseEvent();

            windowData->EventCallback(e);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);
            Base::WindowResizeEvent e = Base::WindowResizeEvent(width, height);
            windowData->Width = width;
            windowData->Height = height;

            windowData->EventCallback(e);
        });
        PG_CORE_INFO("OpenGL window initialization successful.");
    }

    void OpenGLWindow::OnUpdate() {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}