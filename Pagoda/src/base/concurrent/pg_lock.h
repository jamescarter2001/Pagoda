#pragma once

namespace Pagoda::Base {
    class Lock {
    public:
        virtual inline void Acquire() = 0;
        virtual inline bool TryAcquire() = 0;
        virtual inline void Release() = 0;
    };
}