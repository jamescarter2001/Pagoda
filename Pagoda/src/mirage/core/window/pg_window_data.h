#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"
#include "base/event/pg_event.h"

namespace Pagoda::Mirage {
    struct WindowProps {
        std::string Title = "Pagoda";
        int Width = 1280;
        int Height = 720;

        WindowProps() {}

        WindowProps(std::string Title) {
            this->Title = Title;
        }

        WindowProps(std::string Title, int Width, int Height) {
            this->Title = Title;
            this->Width = Width;
            this->Height = Height;
        }
    };

    struct WindowData {
        std::string Title = "";
        int Width = 0, Height = 0;
        bool VSync = false;
        std::function<void(Base::Event& m)> EventCallback = [](Base::Event& m) {
            PG_CORE_WARNING("Application event callback not set!");
        };
    };
}