#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_buffer.h"

namespace Pagoda::Mirage {
    enum class ConstantBufferType {
        CONSTANT_BUFFER_TYPE_MVP,
        CONSTANT_BUFFER_TYPE_TRANSFORM
    };

    template <typename T>
    class ConstantBuffer : public Buffer<T> {
    public:
        ConstantBuffer(T buffer[], int size, ConstantBufferType type) : Buffer(buffer, size) {};
        ConstantBuffer(int size, ConstantBufferType type) : Buffer(size) { PG_CORE_DEBUG("Created constant buffer: [Size {}]", size); };
        virtual ~ConstantBuffer() {};
    };
}