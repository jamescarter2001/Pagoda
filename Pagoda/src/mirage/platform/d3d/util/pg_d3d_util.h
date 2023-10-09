#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    class D3DUtil {
    public:
        static DXGI_FORMAT GetDXGIFormat(unsigned int pgFormat);
    };
}