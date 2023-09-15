#include "pgpch.h"
#include "pg_d3d11_shader.h"

namespace Pagoda::Mirage {
    D3D11Shader::D3D11Shader(ID3D11Device* device, std::string filePath) : Shader(), m_Device(device) {
        this->m_VertexShader = NULL;
        this->m_PixelShader = NULL;

        UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
        #ifdef PG_DEBUG
                flags |= D3DCOMPILE_DEBUG;  // add more debug output
        #endif

        ID3DBlob *vsBlobPtr = NULL, *psBlobPtr = NULL, *errPtr = NULL;

        HRESULT hr = D3DCompileFromFile(
            STR_TO_WSTR(filePath).c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "vs_main",
            "vs_5_0",
            flags,
            0,
            &vsBlobPtr,
            &errPtr);
        if (FAILED(hr)) {
            PG_CORE_WARNING("Failed to compile vertex shader: {} ({})", filePath, std::to_string(hr));
            if (errPtr) {
                OutputDebugStringA((char*)errPtr->GetBufferPointer());
            }
            if (vsBlobPtr) {
                vsBlobPtr->Release();
            }
        }

        hr = D3DCompileFromFile(
            STR_TO_WSTR(filePath).c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "ps_main",
            "ps_5_0",
            flags,
            0,
            &psBlobPtr,
            &errPtr);
        if (FAILED(hr)) {
            if (errPtr) {
                PG_CORE_WARNING("Failed to compile pixel shader: {} ({})", filePath, std::to_string(hr));
                OutputDebugStringA((char*)errPtr->GetBufferPointer());
                errPtr->Release();
            }
            if (psBlobPtr) {
                psBlobPtr->Release();
            }
        }

        HRESULT vshr = m_Device->CreateVertexShader(
            vsBlobPtr->GetBufferPointer(),
            vsBlobPtr->GetBufferSize(),
            NULL,
            &m_VertexShader);

        HRESULT pshr = m_Device->CreatePixelShader(
            psBlobPtr->GetBufferPointer(),
            psBlobPtr->GetBufferSize(),
            NULL,
            &m_PixelShader);

        PG_CORE_ASSERT((vshr & pshr) == S_OK, "Error creating shader(s), check the logs for more details");
    }

    D3D11Shader::~D3D11Shader() {
    }

	void D3D11Shader::Bind() {

    }

    void D3D11Shader::Unbind() {

    }
}