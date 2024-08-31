#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "base/util/pg_strings.h"

#include "mirage/core/shader/pg_shader.h"

#include "mirage/platform/d3d/util/pg_d3d_util.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {
    class D3D11Shader : public Shader {
    public:
        D3D11Shader(std::shared_ptr<D3D11Context> ctx, std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType);
        virtual ~D3D11Shader();

        inline ID3D11VertexShader* GetD3D11VertexShader() const & {
            return this->m_vertexShader;
        }

        inline ID3D11PixelShader* GetD3D11PixelShader() const & {
            return this->m_pixelShader;
        }

        inline ID3DBlob* GetBlob() const & {
            return this->m_blobPtr;
        }

    private:
        void CreateVertexShader(UINT flags);
        void CreatePixelShader(UINT flags);

        void CompileShader(UINT flags, ID3DBlob*& errPtr);

        ID3D11Device* m_device;
        ID3D11DeviceContext* m_deviceContext;

        ID3DBlob* m_blobPtr;

        ID3D11VertexShader* m_vertexShader;
        ID3D11PixelShader* m_pixelShader;

        ID3D11InputLayout* m_inputLayout;
    };
}
