#pragma once

namespace Pagoda {
	extern CApplication* CreateApplication();
}

int main(int argc, char** argv) {
	Pagoda::Base::CLog::Init();
	PG_CORE_INFO("Initialized log.");

	auto app = Pagoda::CreateApplication();
	Pagoda::Base::CLog::SetClientName(app->GetName());
	PG_CORE_DEBUG("Client name updated: {}", app->GetName());

	app->Run();
	delete app;
}