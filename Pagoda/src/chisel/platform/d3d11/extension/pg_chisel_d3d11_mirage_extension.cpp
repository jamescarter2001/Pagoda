#include "pgpch.h"
#include "pg_chisel_d3d11_mirage_extension.h"

#include "imgui.h"

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Pagoda::Chisel {

	ChiselD3D11MirageExtension::ChiselD3D11MirageExtension(std::shared_ptr<Mirage::D3D11Context> ctx) : m_ctx(ctx) {

    }

    ChiselD3D11MirageExtension::~ChiselD3D11MirageExtension() {
    }

    void ChiselD3D11MirageExtension::Init() {
        ImGui_ImplWin32_Init(m_ctx->GetWindow());
        ImGui_ImplDX11_Init(m_ctx->GetDevice(), m_ctx->GetDeviceContext());
    }

    void ChiselD3D11MirageExtension::BeforeUpdate() {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
    }

    void ChiselD3D11MirageExtension::OnUpdate() {
        ImGuiIO& io = ImGui::GetIO();

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void ChiselD3D11MirageExtension::ShutDown() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
    }

    LRESULT ChiselD3D11MirageExtension::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        return ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
    }

}