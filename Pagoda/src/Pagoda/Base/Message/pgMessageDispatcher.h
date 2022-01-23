#pragma once
#include "pgCore.h"
#include "pgMessage.h"

namespace Pagoda::Base {
    class PAGODA_API MessageDispatcher {
    public:
        MessageDispatcher(Message& m) : m_Message(m) {}

        // methods using templates must be fully declared in header to avoid linker error.
        template <typename T>
        inline void Dispatch(std::function<bool(T&)> func) const {
            if (dynamic_cast<T*>(&m_Message) != nullptr) {
                PG_CORE_TRACE("Processing message: {}", this->m_Message.ToString());
                this->m_Message.m_IsHandled = func((T&)this->m_Message);
            }
        }

    private:
        Message& m_Message;
    };
}