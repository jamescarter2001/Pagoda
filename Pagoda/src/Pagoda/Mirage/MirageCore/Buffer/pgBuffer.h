#include "pgpch.h"

namespace Pagoda::Mirage {
    class Buffer {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    protected:
        unsigned int m_RendererID;
    };
}