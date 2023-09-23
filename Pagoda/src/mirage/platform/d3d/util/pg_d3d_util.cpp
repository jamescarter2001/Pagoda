#include "pgpch.h"
#include "pg_d3d_util.h"

namespace Pagoda::Mirage {
    DXGI_FORMAT D3DUtil::GetDXGIFormat(unsigned int pgFormat) {
        switch (pgFormat) {
            case PG_VECTOR3:
                return DXGI_FORMAT_R32G32B32_FLOAT;
            case PG_VECTOR4:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
            default:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
    }
}