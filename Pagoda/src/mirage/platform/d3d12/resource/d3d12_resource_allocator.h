#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

class D3D12ResourceAllocator {
public:
    D3D12ResourceAllocator();
    virtual ~D3D12ResourceAllocator();
private:
    void CopyAndTransition(ID3D12Resource* dest, ID3D12Resource* src);

    ComPtr<ID3D12Device> m_device;
};