#include "pgpch.h"
#include "pgOpenGLWindow.h"

namespace Pagoda::Mirage {

    OpenGLWindow::OpenGLWindow(const WindowProps& props) : Window(props) {
        this->m_Window = nullptr;
        Init();
    }

    Window* Window::Create(const WindowProps& props) {
        return new OpenGLWindow(props);
    }

    void OpenGLWindow::Init() {
        PG_CORE_INFO("Creating new OpenGL window: {0} ({1}, {2})", this->m_WindowData.Title, this->m_WindowData.Width, this->m_WindowData.Height);
        PG_CORE_ASSERT(glfwInit(), "GLFW initialization error.");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        // Disable compatibility mode.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        m_Window = glfwCreateWindow(this->m_WindowData.Width, this->m_WindowData.Height, this->m_WindowData.Title.c_str(), NULL, NULL);
        PG_CORE_ASSERT(this->m_Window, "Unable to create OpenGL window context.");

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
    }

    void OpenGLWindow::OnUpdate() {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}