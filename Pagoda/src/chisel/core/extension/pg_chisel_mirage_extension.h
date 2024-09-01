#pragma once

#include "mirage/core/extension/pg_extension.h"

namespace Pagoda::Chisel {
    class ChiselMirageExtension : public Mirage::Extension {
    public:
        ChiselMirageExtension();
        virtual ~ChiselMirageExtension();

        virtual void Init() override;
        virtual void OnUpdate() override;
    };
}