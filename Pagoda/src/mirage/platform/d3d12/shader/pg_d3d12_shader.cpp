#include "pgpch.h"
#include "pg_d3d12_shader.h"

namespace Pagoda::Mirage {
    D3D12Shader::D3D12Shader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType) : Shader(filePath, vertexBufferLayout, shaderType) {
        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        #ifdef PG_DEBUG
        flags |= D3DCOMPILE_DEBUG;  // add more debug output
        flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
        #endif

        this->CompileShader(flags);

        if (this->IsVertexShader()) {
            this->CreateVertexShader(flags);

            auto elements = this->m_vertexBufferLayout.GetElements();

            int index = 0;
            for (VertexBufferElement& e : elements) {
                // TODO: Rework to take vertex element size
                this->elementDesc.push_back({e.name.c_str(), 0, D3DUtil::GetDXGIFormat(e.platformFormat), 0, (index > 0 ? D3D11_APPEND_ALIGNED_ELEMENT : 0), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0});
                index++;
            }
        } else {
            this->CreatePixelShader(flags);
        }
    }

    D3D12Shader::~D3D12Shader() {

    }

    void D3D12Shader::CreateVertexShader(UINT flags) {
    }

    void D3D12Shader::CreatePixelShader(UINT flags) {
    }

    void D3D12Shader::CompileShader(UINT flags) {
        ComPtr<ID3DBlob> errPtr;

        HRESULT hr = D3DCompileFromFile(
            Base::Strings::STR_TO_WSTR(this->m_FilePath).c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            (this->m_ShaderType == ShaderType::SHADER_TYPE_VERTEX ? "vs_main" : "ps_main"),
            (this->m_ShaderType == ShaderType::SHADER_TYPE_VERTEX ? "vs_5_0" : "ps_5_0"),
            flags,
            0,
            &this->m_shaderBlob,
            &errPtr);
        if (hr != S_OK) {
            if (errPtr) {
                PG_CORE_WARNING("Failed to compile {} shader: {} ({})", this->GetShaderType(), this->m_FilePath, std::to_string(hr));
                OutputDebugStringA((char*)errPtr->GetBufferPointer());
                PG_CORE_WARNING((char*)errPtr->GetBufferPointer());
                errPtr->Release();
            }
            if (this->m_shaderBlob) {
                this->m_shaderBlob->Release();
            }
        }
    }
}