#include <Pagoda.h>

class SandboxApp : public Pagoda::Application {
public:
	// Application default constructor called automatically.
	SandboxApp(std::string& name) : Pagoda::Application(name) {
		PG_TRACE("Created new engine application.");
		//PushLayer(new ExampleLayer());
		//PushOverlay(new GameEngine::ReddogLayer());
	}
};

Pagoda::Application* Pagoda::CreateApplication() {
	std::string name("TestApp");
	return new SandboxApp(name);
}