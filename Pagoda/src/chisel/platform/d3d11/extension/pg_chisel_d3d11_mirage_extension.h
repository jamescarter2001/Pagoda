#pragma once
#include "pgpch.h"

#include "mirage/platform/d3d/extension/pg_windows_extension.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Chisel {
    class ChiselD3D11MirageExtension : public Mirage::WindowsExtension {
    public:

        ChiselD3D11MirageExtension(std::shared_ptr<Mirage::D3D11Context> ctx);
        virtual ~ChiselD3D11MirageExtension();

        virtual void Init() override;
        virtual void BeforeUpdate() override;
        virtual void OnUpdate() override;
        virtual void ShutDown() override;

        virtual LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    private:
        std::shared_ptr<Mirage::D3D11Context> m_ctx;
    };
}