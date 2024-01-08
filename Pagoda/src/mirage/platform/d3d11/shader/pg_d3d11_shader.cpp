#include "pgpch.h"
#include "pg_d3d11_shader.h"

namespace Pagoda::Mirage {
    D3D11Shader::D3D11Shader(D3D11Context* ctx, std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType)
        : Shader(filePath, vertexBufferLayout, shaderType) {
        this->m_device = ctx->GetDevice();
        this->m_deviceContext = ctx->GetDeviceContext();

        this->m_vertexShader = NULL;
        this->m_pixelShader = NULL;
        this->m_blobPtr = NULL;
        this->m_inputLayout = NULL;

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef PG_DEBUG
        flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif

        if (this->IsVertexShader()) {
            this->CreateVertexShader(flags);
            PG_CORE_ASSERT(this->m_pixelShader == NULL, "Pixel shader should be null!");
        } else {
            this->CreatePixelShader(flags);
            PG_CORE_ASSERT(this->m_vertexShader == NULL, "Vertex shader should be null!");
        }
    }

    D3D11Shader::~D3D11Shader() {
        this->m_blobPtr->Release();

        if (this->IsVertexShader()) {
            this->m_vertexShader->Release();
        } else {
            this->m_pixelShader->Release();
        }
    }

    void D3D11Shader::CreateVertexShader(UINT flags) {
        ID3DBlob* errPtr = NULL;

        this->CompileShader(flags, errPtr);

        HRESULT vshr = m_device->CreateVertexShader(
            this->m_blobPtr->GetBufferPointer(),
            this->m_blobPtr->GetBufferSize(),
            NULL,
            &m_vertexShader);

        PG_CORE_ASSERT(vshr == S_OK, "Failed to create vertex shader");

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
            this->m_blobPtr->GetBufferPointer(),
            this->m_blobPtr->GetBufferSize(),
            &this->m_inputLayout);

        PG_CORE_ASSERT(ilhr == S_OK, "Error creating input layout");
    }

    void D3D11Shader::CreatePixelShader(UINT flags) {
        ID3DBlob* errPtr = NULL;

        this->CompileShader(flags, errPtr);

        HRESULT pshr = m_device->CreatePixelShader(
            this->m_blobPtr->GetBufferPointer(),
            this->m_blobPtr->GetBufferSize(),
            NULL,
            &m_pixelShader);
    }

    void D3D11Shader::CompileShader(UINT flags, ID3DBlob*& errPtr) {
        HRESULT hr = D3DCompileFromFile(
            STR_TO_WSTR(this->m_FilePath).c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            (this->m_ShaderType == ShaderType::SHADER_TYPE_VERTEX ? "vs_main" : "ps_main"),
            (this->m_ShaderType == ShaderType::SHADER_TYPE_VERTEX ? "vs_5_0" : "ps_5_0"),
            flags,
            0,
            &this->m_blobPtr,
            &errPtr);
        if (hr != S_OK) {
            if (errPtr) {
                PG_CORE_WARNING("Failed to compile {} shader: {} ({})", this->GetShaderType(), this->m_FilePath, std::to_string(hr));
                OutputDebugStringA((char*)errPtr->GetBufferPointer());
                PG_CORE_WARNING((char*)errPtr->GetBufferPointer());
                errPtr->Release();
            }
            if (this->m_blobPtr) {
                this->m_blobPtr->Release();
            }
        }
    }
}
