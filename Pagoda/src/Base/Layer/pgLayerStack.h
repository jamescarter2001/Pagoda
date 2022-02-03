#pragma once
#include "Base/Layer/pgLayer.h"
#include "Base/Log/pgLog.h"
#include "pgCore.h"

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