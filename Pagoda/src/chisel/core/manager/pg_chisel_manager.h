#pragma once

#include "base/manager/pg_abstract_manager.h"

namespace Pagoda::Chisel {
    class ChiselManager : public Base::AbstractManager {
    public:
        ChiselManager();
        virtual ~ChiselManager();

        virtual void Init() override;
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;

        virtual void ShutDown() override;
    };
}