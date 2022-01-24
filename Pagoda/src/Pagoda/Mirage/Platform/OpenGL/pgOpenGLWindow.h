#pragma once
#include "pgpch.h"

#include "GLFW/glfw3.h"

#include "Base/Event/pgApplicationEvent.h"
#include "Mirage/MirageCore/pgWindow.h"

namespace Pagoda::Mirage {
    class OpenGLWindow : public Window {
    public:
        OpenGLWindow(const WindowProps& props);
        virtual void OnUpdate() override;
        virtual void Init() override;
    private:
        GLFWwindow* m_Window;
    };
};