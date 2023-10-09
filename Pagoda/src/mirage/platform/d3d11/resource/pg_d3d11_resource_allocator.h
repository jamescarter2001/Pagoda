#pragma once
#include "pgpch.h"

#include "mirage/platform/d3d11/context/pg_d3d11_context.h"

namespace Pagoda::Mirage {
    class D3D11ResourceAllocator {
    public:
        D3D11ResourceAllocator(D3D11Context context);
        virtual ~D3D11ResourceAllocator();

        void AllocateDefault(ID3D11Buffer** ptr, void* buff, int size, D3D11_BIND_FLAG bufferType);
        void AllocateReadWrite(ID3D11Buffer** ptr, int size, D3D11_BIND_FLAG bufferType);

    private:
        D3D11Context m_context;
    };
}