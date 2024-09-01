#pragma once
#include "pgpch.h"

#include "base/log/pg_log.h"

#include "universe/layer/pg_layer.h"

namespace Pagoda::Universe {
    class PAGODA_API LayerStack {
    public:
        LayerStack(ApplicationContext& ctx);
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
        ApplicationContext& m_ctx;

        std::vector<Layer*> m_Layers;
        std::vector<Layer*>::iterator m_LayerInsert;
    };
}