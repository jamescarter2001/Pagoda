#pragma once

#include "chisel/core/extension/pg_chisel_mirage_extension.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"

namespace Pagoda::Chisel {
    class ChiselD3D12MirageExtension : public ChiselMirageExtension {
    public:
        ChiselD3D12MirageExtension(std::shared_ptr<Mirage::D3D12Context> ctx);
        virtual ~ChiselD3D12MirageExtension();

        virtual void Init() override;
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;
        virtual void ShutDown() override;

    private:
        std::shared_ptr<Mirage::D3D12Context> m_ctx;
    };
}
