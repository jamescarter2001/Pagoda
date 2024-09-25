#pragma once

#include "mirage/core/extension/pg_extension.h"

namespace Pagoda::Mirage {
    class WindowsExtension : public Extension {
    public:
        virtual LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    };
}