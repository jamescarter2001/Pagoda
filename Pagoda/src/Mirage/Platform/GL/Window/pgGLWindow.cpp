#include "pgpch.h"

#include "pgGLWindow.h"

namespace Pagoda::Mirage {

    GLWindow::GLWindow(const WindowProps& props) : Window(props) {
        this->m_Window = nullptr;

        // TODO - Export to ApplicationSetting.lua
        #ifdef PG_PLATFORM_MACOS
        this->m_GLVersion = GLVersion(3, 2, GLFW_OPENGL_CORE_PROFILE);
        #endif

        Init();
    }

    GLWindow::~GLWindow() {
        glfwDestroyWindow(m_Window);
    }

    Window* Window::Create(const WindowProps& props) {
        return new GLWindow(props);
    }

    void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        fprintf(stderr, "[OpenGL] %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type, severity, message);
    }

    void GLWindow::Init() {
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

        PG_CORE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "GLAD initialization error.");

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

        /* Causes black screen with 2D
        
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);

        */

        glDebugMessageCallback(DebugCallback, 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        PG_CORE_INFO("OpenGL window initialization successful.");
    }

    void GLWindow::OnUpdate() {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}