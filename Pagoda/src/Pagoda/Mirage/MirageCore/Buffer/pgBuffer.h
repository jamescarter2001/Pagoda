#include "pgpch.h"

namespace Pagoda::Mirage {
    class Buffer {
    public:
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
    protected:
        unsigned int m_RendererID;
    };
}