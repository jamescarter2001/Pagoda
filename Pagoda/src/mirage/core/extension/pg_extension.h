#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
    class PAGODA_API Extension {
    public:
        Extension() {};
        virtual ~Extension() {};

        virtual void Init() = 0;
        virtual void BeforeUpdate() = 0;
        virtual void OnUpdate() = 0;
        virtual void ShutDown() = 0;
    };
}