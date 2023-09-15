#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "base/util/pg_util.h"

#include "mirage/core/shader/pg_shader.h"

namespace Pagoda::Mirage {
    class D3D11Shader : public Shader {
    public:
        D3D11Shader(ID3D11Device* device, std::string filePath);
        virtual ~D3D11Shader();

        virtual void Bind() override;
        virtual void Unbind() override;

    private:
        ID3D11Device* m_Device;

        ID3D11VertexShader* m_VertexShader;
        ID3D11PixelShader* m_PixelShader;
    };
}
