#include "pgpch.h"
#include "pgLayer.h"

namespace Pagoda::Base {
    Layer::Layer(const std::string& name) {
        this->m_Name = name;
    }

    Layer::~Layer() {}
}