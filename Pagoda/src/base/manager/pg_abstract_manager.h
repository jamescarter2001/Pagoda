#pragma once
#include "pgpch.h"

namespace Pagoda::Base {
    class AbstractManager {
    public:
        AbstractManager() {}
        virtual ~AbstractManager() {}

        virtual void Init() = 0;
        virtual void ShutDown() = 0;
    };
}
