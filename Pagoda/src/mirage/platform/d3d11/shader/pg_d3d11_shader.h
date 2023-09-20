#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "base/util/pg_util.h"

#include "mirage/core/shader/pg_shader.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {
    class D3D11Shader : public Shader {
    public:
        D3D11Shader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType);
        virtual ~D3D11Shader();

        virtual void Bind() override;
        virtual void Unbind() override;

        DXGI_FORMAT GetDXGIFormat(unsigned int format);
    private:
        void CreateVertexShader(UINT flags);
        void CreatePixelShader(UINT flags);

        void CompileShader(UINT flags, ID3DBlob*& errPtr);

        inline std::string GetShaderType() {
            return this->m_ShaderType == ShaderType::SHADER_TYPE_VERTEX ? "VERTEX" : "PIXEL";
        }

        inline bool IsVertexShader() {
            return this->m_ShaderType == ShaderType::SHADER_TYPE_VERTEX;
        }

        ID3D11Device* m_Device;
        ID3D11DeviceContext* m_DeviceContext;

        ID3DBlob* m_BlobPtr;

        ID3D11VertexShader* m_VertexShader;
        ID3D11PixelShader* m_PixelShader;

        ID3D11InputLayout* m_InputLayout;
    };
}
