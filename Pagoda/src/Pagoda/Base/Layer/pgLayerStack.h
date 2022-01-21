#pragma once
#include "pgCore.h"
#include "Base/Log/pgLog.h"
#include "Base/Layer/pgLayer.h"

namespace Pagoda::Base {
	class PAGODA_API CLayerStack {
	public:
		CLayerStack();
		virtual ~CLayerStack();

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