#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"
#include "base/util/pg_util.h"

#include "mirage/core/shader/pg_shader.h"

#include "mirage/platform/d3d/util/pg_d3d_util.h"

namespace Pagoda::Mirage {
    class D3D12Shader : public Shader {
    public:
        D3D12Shader(std::string& filePath, VertexBufferLayout& vertexBufferLayout, ShaderType shaderType);
        virtual ~D3D12Shader();

        inline ComPtr<ID3DBlob> GetBlob() const& {
            return this->m_shaderBlob;
        }

    private:
        void CompileShader(UINT flags);

        void CreateVertexShader(UINT flags);
        void CreatePixelShader(UINT flags);

        std::vector<D3D12_INPUT_ELEMENT_DESC> elementDesc;
        ComPtr<ID3DBlob> m_shaderBlob;
    };
}