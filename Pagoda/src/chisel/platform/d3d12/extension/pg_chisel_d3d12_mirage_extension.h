#pragma once

#include "mirage/platform/d3d/extension/pg_windows_extension.h"

#include "mirage/platform/d3d12/context/pg_d3d12_context.h"

namespace Pagoda::Chisel {
    class ChiselD3D12MirageExtension : public Mirage::WindowsExtension {
    public:
        ChiselD3D12MirageExtension(std::shared_ptr<Mirage::D3D12Context> ctx);
        virtual ~ChiselD3D12MirageExtension();

        virtual void Init() override;
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;
        virtual void ShutDown() override;

        virtual LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    private:
        std::shared_ptr<Mirage::D3D12Context> m_ctx;
    };
}
