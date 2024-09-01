#include "pgpch.h"
#include "pg_layer.h"

namespace Pagoda::Universe {
    Layer::Layer(const std::string& name) {
        this->m_Name = name;
    }

    Layer::~Layer() {}
}