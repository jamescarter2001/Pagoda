#pragma once

namespace Pagoda {
	extern Application* CreateApplication();
}

int main(int argc, char** argv) {
	Pagoda::Base::Log::Init();
	PG_CORE_INFO("Initialized log.");

	auto app = Pagoda::CreateApplication();
	Pagoda::Base::Log::SetClientName(app->GetName());

	app->Run();
}