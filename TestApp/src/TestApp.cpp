#include <Pagoda.h>

class SandboxApp : public Pagoda::pgApplication {
public:
	// Application default constructor called automatically.
	SandboxApp(std::string& name) : Pagoda::pgApplication(name) {
		PG_TRACE("Created new engine application.");
		//PushLayer(new ExampleLayer());
		//PushOverlay(new GameEngine::ReddogLayer());
	}
};

Pagoda::pgApplication* Pagoda::CreateApplication() {
	std::string name("TestApp");
	return new SandboxApp(name);
}