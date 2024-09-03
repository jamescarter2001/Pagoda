#include "pgpch.h"
#include "pg_chisel_d3d12_mirage_extension.h"

#include <d3d12.h>

#include "imgui.h"

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx12.h"

namespace Pagoda::Chisel {

    ChiselD3D12MirageExtension::ChiselD3D12MirageExtension(std::shared_ptr<Mirage::D3D12Context> ctx) : m_ctx(ctx) {

    }

    ChiselD3D12MirageExtension::~ChiselD3D12MirageExtension() {

    }

    void ChiselD3D12MirageExtension::Init() {
        ChiselMirageExtension::Init();

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(m_ctx->GetWindow());
        ImGui_ImplDX12_Init(m_ctx->GetDevice().Get(), 3 /* NUM_FRAMES_IN_FLIGHT */,
                            DXGI_FORMAT_R8G8B8A8_UNORM, m_ctx->GetSrvDescHeap().Get(),
                            m_ctx->GetSrvDescHeap()->GetCPUDescriptorHandleForHeapStart(),
                            m_ctx->GetSrvDescHeap()->GetGPUDescriptorHandleForHeapStart());
    }

    void ChiselD3D12MirageExtension::BeforeUpdate() {
        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
    }

    void ChiselD3D12MirageExtension::OnUpdate() {
        ChiselMirageExtension::OnUpdate();

        ImGuiIO& io = ImGui::GetIO();

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::RenderPlatformWindowsDefault(nullptr, (void*)m_ctx->GetCommandList().Get());
        }
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), m_ctx->GetCommandList().Get());
    }

    void ChiselD3D12MirageExtension::ShutDown() {

    }
}