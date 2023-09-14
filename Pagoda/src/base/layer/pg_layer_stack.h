#pragma once
#include "base/layer/pg_layer.h"
#include "base/log/pg_log.h"
#include "pg_core.h"

namespace Pagoda::Base {
    class PAGODA_API LayerStack {
    public:
        LayerStack();
        virtual ~LayerStack();

        void PushLayer(Layer* layer);
        void PopLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopOverlay(Layer* layer);

        inline std::vector<Layer*>::iterator begin() {
            return m_Layers.begin();
        }

        inline std::vector<Layer*>::iterator end() {
            return m_Layers.end();
        }

    private:
        std::vector<Layer*> m_Layers;
        std::vector<Layer*>::iterator m_LayerInsert;
    };
}