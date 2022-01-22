#include "pgpch.h"
#include "pgOpenGLWindow.h"

namespace Pagoda::Mirage {

    OpenGLWindow::OpenGLWindow(const WindowProps& props) : Window(props) {
        Init();
    }

    Window* Window::Create(const WindowProps& props) {
        return new OpenGLWindow(props);
    }

    void OpenGLWindow::Init() {
        
    }
}