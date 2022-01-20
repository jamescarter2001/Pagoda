#pragma once
#include "pgCore.h"
#include "Base/Log/pgLog.h"
#include "Base/Layer/pgLayer.h"

namespace Pagoda::Base {
	class PAGODA_API CLayerStack {
	public:
		CLayerStack();
		virtual ~CLayerStack();

		void PushLayer(CLayer* layer);
		void PopLayer(CLayer* layer);
		void PushOverlay(CLayer* layer);
		void PopOverlay(CLayer* layer);

		inline std::vector<CLayer*>::iterator begin() {
			return m_Layers.begin();
		}

		inline std::vector<CLayer*>::iterator end() {
			return m_Layers.end();
		}

	private:
		std::vector<CLayer*> m_Layers;
		std::vector<CLayer*>::iterator m_LayerInsert;
	};
}