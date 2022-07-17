#pragma once
#include "pgpch.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Base/Event/pgApplicationEvent.h"
#include "Mirage/MirageCore/Window/pgWindow.h"

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

    class GLWindow : public Window {
    public:
        GLWindow(const WindowProps& props);
        virtual ~GLWindow();
        virtual void OnUpdate() override;
        virtual void Init() override;

        GLVersion m_GLVersion;
    private:
        GLFWwindow* m_Window;
    };
};
