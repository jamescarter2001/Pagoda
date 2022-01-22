#pragma once
#include "pgpch.h"
#include "Base/Message/pgMessage.h"

namespace Pagoda::Mirage {
	struct WindowProps {
		std::string Title;
		int Width;
		int Height;

		WindowProps() {
			this->Title = "Pagoda";
			this->Width = 1280;
			this->Height = 720;
		}

		WindowProps(int Width, int Height, std::string Title) {
			this->Title = Title;
			this->Width = Width;
			this->Height = Height;
		}
	};

	class Window {
		public:
		Window(const WindowProps& props) {
			m_WindowData.Title = props.Title;
			m_WindowData.Width = props.Width;
			m_WindowData.Height = props.Height;
		}
		virtual Window* Create(const WindowProps& props) = 0;
		virtual void Init() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnMessage(const Base::Message& m) = 0;

		inline int GetWidth() {
			return m_WindowData.Width;
		}
		inline int GetHeight() {
			return m_WindowData.Height;
		}
		protected:
		WindowProps m_WindowData;
	};
}