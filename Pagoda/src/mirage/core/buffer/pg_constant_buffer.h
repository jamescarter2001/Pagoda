#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "mirage/core/buffer/pg_buffer.h"

namespace Pagoda::Mirage {
    template <typename T>
    class ConstantBuffer : public Buffer<T> {
    public:
        ConstantBuffer(T buffer[], int size) : Buffer(buffer, size) { PG_CORE_DEBUG("Created constant buffer: [Size {}]", size); };
        virtual ~ConstantBuffer() {};

        virtual void Write(T buffer[]) const = 0;
    };
}