#include "pgpch.h"
#include "pg_d3d12_pipeline_state.h"

namespace Pagoda::Mirage {
    D3D12PipelineState::D3D12PipelineState(std::shared_ptr<D3D12Context> context, Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) : PipelineState(vertexShader, fragmentShader, vertexBufferLayout), m_context(context) {
        auto elements = this->m_vertexBufferLayout.GetElements();

        std::vector<D3D12_INPUT_ELEMENT_DESC> elementDesc;
        int index = 0;
        for (VertexBufferElement& e : elements) {
            // TODO: Rework to take vertex element size
            elementDesc.push_back({e.name.c_str(), 0, D3DUtil::GetDXGIFormat(e.platformFormat), 0, (index > 0 ? D3D12_APPEND_ALIGNED_ELEMENT : 0), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
            index++;
        }

        D3D12Shader* vs = dynamic_cast<D3D12Shader*>(this->m_vertexShader);
        D3D12Shader* ps = dynamic_cast<D3D12Shader*>(this->m_fragmentShader);

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = {&elementDesc[0], (unsigned int)elementDesc.size()};
        psoDesc.pRootSignature = this->m_context->GetRootSignature().Get();
        psoDesc.VS = {reinterpret_cast<UINT8*>(vs->GetBlob()->GetBufferPointer()), vs->GetBlob()->GetBufferSize()};
        psoDesc.PS = {reinterpret_cast<UINT8*>(ps->GetBlob()->GetBufferPointer()), ps->GetBlob()->GetBufferSize()};
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState.DepthEnable = FALSE;
        psoDesc.DepthStencilState.StencilEnable = FALSE;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;

        if (this->m_context->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)) != S_OK) {
            PG_CORE_ERROR("Failed to create Graphics Pipeline State");
        }
    }

    D3D12PipelineState::~D3D12PipelineState() {

    }

    void D3D12PipelineState::Bind() const {
        this->m_context->GetCommandList()->SetPipelineState(this->m_pipelineState.Get());
    }

    void D3D12PipelineState::Unbind() const {
    }
}