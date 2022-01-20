#include <Pagoda.h>

class SandboxApp : public Pagoda::CApplication {
public:
	// Application default constructor called automatically.
	SandboxApp(std::string& name) : Pagoda::CApplication(name) {
		PG_TRACE("Created new engine application.");
		//PushLayer(new ExampleLayer());
		//PushOverlay(new GameEngine::ReddogLayer());
	}
};

Pagoda::CApplication* Pagoda::CreateApplication() {
	std::string name("TestApp");
	return new SandboxApp(name);
}