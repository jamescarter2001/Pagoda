#include "pgpch.h"
#include "pgLayerStack.h"

namespace Pagoda::Base {

	CLayerStack::CLayerStack() {
		this->m_LayerInsert = this->m_Layers.begin();
	}

	CLayerStack::~CLayerStack() {
		for (CLayer* layer : this->m_Layers) {
			delete layer;
		}
	}

	void CLayerStack::PushLayer(CLayer* layer) {
		this->m_LayerInsert = this->m_Layers.emplace(m_LayerInsert, layer);
		layer->OnAttach();
	}

	void CLayerStack::PopLayer(CLayer* layer) {
		std::vector<CLayer*>::iterator it = std::find(this->m_Layers.begin(), this->m_Layers.end(), layer);
		if (it != this->m_Layers.end()) {
			this->m_Layers.erase(it);
			this->m_LayerInsert--;
		} else {
			PG_CORE_WARNING("Cannot pop invalid layer: {}", layer->GetName());
		}
	}

	void CLayerStack::PushOverlay(CLayer* layer) {
		this->m_Layers.emplace_back(layer);
	}

	void CLayerStack::PopOverlay(CLayer* layer) {
		std::vector<CLayer*>::iterator it = std::find(this->m_Layers.begin(), this->m_Layers.end(), layer);
		if (it != this->m_Layers.end()) {
			this->m_Layers.erase(it);
		} else {
			PG_CORE_WARNING("Cannot pop invalid overlay: {}", layer->GetName());
		}
	}
}