#pragma once
#include "pgpch.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "base/log/pg_log.h"

#include "base/event/pg_application_event.h"

#include "mirage/core/window/pg_window.h"

namespace Pagoda::Mirage {
    struct GLVersion {
        int Major = 4;
        int Minor = 6;
        unsigned int Profile = GLFW_OPENGL_CORE_PROFILE;

        GLVersion() {}

        GLVersion(int major, int minor, unsigned int profile) {
            this->Major = major;
            this->Minor = minor;
            this->Profile = profile;
        }
    };

    void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

    class GLWindow : public Window {
    public:
        GLWindow(const WindowProps& props);
        virtual ~GLWindow();
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;
        virtual void Init() override;

        GLVersion m_GLVersion;
    private:
        GLFWwindow* m_Window;
    };
};
