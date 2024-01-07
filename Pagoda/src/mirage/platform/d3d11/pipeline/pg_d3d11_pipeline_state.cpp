#include "pgpch.h"
#include "pg_d3d11_pipeline_state.h"

namespace Pagoda::Mirage {
    D3D11PipelineState::D3D11PipelineState(D3D11Context* ctx, Shader* vertexShader, Shader* fragmentShader, VertexBufferLayout& vertexBufferLayout) : PipelineState(vertexShader, fragmentShader, vertexBufferLayout) {
        this->m_device = ctx->GetDevice();
        this->m_deviceContext = ctx->GetDeviceContext();

        this->m_inputLayout = NULL;

        D3D11Shader* vs = dynamic_cast<D3D11Shader*>(this->m_vertexShader);
        D3D11Shader* ps = dynamic_cast<D3D11Shader*>(this->m_fragmentShader);

        if (vs != nullptr && ps != nullptr) {
            this->m_vs = vs->GetD3D11VertexShader();
            this->m_ps = ps->GetD3D11PixelShader();
        } else {
            PG_CORE_ERROR("Failed to cast Direct3D11 shaders");
            this->m_vs = NULL;
            this->m_ps = NULL;
            return;
        }

        std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
        auto elements = this->m_vertexBufferLayout.GetElements();

        int index = 0;
        for (VertexBufferElement& e : elements) {
            desc.push_back({e.name.c_str(), 0, D3DUtil::GetDXGIFormat(e.platformFormat), 0, (index > 0 ? D3D11_APPEND_ALIGNED_ELEMENT : 0), D3D11_INPUT_PER_VERTEX_DATA, 0});
            index++;
        }

        HRESULT ilhr = this->m_device->CreateInputLayout(
            &desc[0],
            (UINT)elements.size(),
            vs->GetBlob()->GetBufferPointer(),
            vs->GetBlob()->GetBufferSize(),
            &this->m_inputLayout);

        PG_CORE_ASSERT(ilhr == S_OK, "Failed to create Input Layout");
    }

    D3D11PipelineState::~D3D11PipelineState() {

    }

    void D3D11PipelineState::Bind() const {
        this->m_deviceContext->IASetInputLayout(this->m_inputLayout);
        this->m_deviceContext->VSSetShader(this->m_vs, NULL, 0);
        this->m_deviceContext->PSSetShader(this->m_ps, NULL, 0);
    }

    void D3D11PipelineState::Unbind() const {

    }
}