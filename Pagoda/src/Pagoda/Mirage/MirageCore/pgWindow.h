#pragma once
#include "pgpch.h"

namespace Pagoda::Mirage {
	struct WindowProps {
		int Width;
		int Height;
		std::string Title;

		WindowProps(int Width, int Height, std::string Title) {
			this->Width = Width;
			this->Height = Height;
			this->Title = Title;
		}
	};
}