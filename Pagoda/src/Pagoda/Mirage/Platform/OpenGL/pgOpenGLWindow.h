#pragma once
#include "pgpch.h"
#include "Mirage/MirageCore/pgWindow.h"

namespace Pagoda::Mirage {
    class OpenGLWindow : public Window {
    public:
        OpenGLWindow(const WindowProps& props);
        virtual Window* Create(const WindowProps& props) override;
        virtual void OnUpdate() override;
        virtual void OnMessage(const Base::Message& m) override;
        virtual void Init() override;
    };
};