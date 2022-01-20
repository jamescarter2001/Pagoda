#include "pgpch.h"
#include "pgLayer.h" 

namespace Pagoda::Base {
	CLayer::CLayer(const std::string& name) {
		this->m_Name = name;
	}

	CLayer::~CLayer() {}
}