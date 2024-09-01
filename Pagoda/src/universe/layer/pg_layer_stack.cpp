#include "pgpch.h"
#include "pg_layer_stack.h"

namespace Pagoda::Universe {

    LayerStack::LayerStack(ApplicationContext& ctx) : m_ctx(ctx) {
        this->m_LayerInsert = this->m_Layers.begin();
    }

    LayerStack::~LayerStack() {
        for (Layer* layer : this->m_Layers) {
            delete layer;
        }
    }

    void LayerStack::PushLayer(Layer* layer) {
        this->m_LayerInsert = this->m_Layers.emplace(m_LayerInsert, layer);
        layer->OnAttach(m_ctx);
    }

    void LayerStack::PopLayer(Layer* layer) {
        std::vector<Layer*>::iterator it = std::find(this->m_Layers.begin(), this->m_Layers.end(), layer);
        if (it != this->m_Layers.end()) {
            this->m_Layers.erase(it);
            this->m_LayerInsert--;
        } else {
            PG_CORE_WARNING("Cannot pop invalid layer: {}", layer->GetName());
        }
    }

    void LayerStack::PushOverlay(Layer* layer) {
        this->m_Layers.emplace_back(layer);
    }

    void LayerStack::PopOverlay(Layer* layer) {
        std::vector<Layer*>::iterator it = std::find(this->m_Layers.begin(), this->m_Layers.end(), layer);
        if (it != this->m_Layers.end()) {
            this->m_Layers.erase(it);
        } else {
            PG_CORE_WARNING("Cannot pop invalid overlay: {}", layer->GetName());
        }
    }
}