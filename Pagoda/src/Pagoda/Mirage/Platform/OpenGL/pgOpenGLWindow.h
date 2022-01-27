#pragma once
#include "pgpch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Base/Event/pgApplicationEvent.h"
#include "Mirage/MirageCore/Window/pgWindow.h"

namespace Pagoda::Mirage {
    struct GLVersion {
        int Major = 3;
        int Minor = 2;
        unsigned int Profile = GLFW_OPENGL_CORE_PROFILE;

        GLVersion() {}

        GLVersion(int major, int minor, unsigned int profile) {
            this->Major = major;
            this->Minor = minor;
            this->Profile = profile;
        }
    };

    class OpenGLWindow : public Window {
    public:
        OpenGLWindow(const WindowProps& props);
        virtual ~OpenGLWindow();
        virtual void OnUpdate() override;
        virtual void Init() override;

        GLVersion m_GLVersion;
    private:
        GLFWwindow* m_Window;
    };
};
