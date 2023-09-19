#include "pagoda.h"
#include "entry_point.h"

#include "test_layer.h"

class TestApp : public Pagoda::Application {
public:
    TestApp(std::string& name) : Pagoda::Application(name) {
        PG_TRACE("Created new engine application");
        PushLayer(new TestLayer("TestLayer"));
        // PushOverlay(new GameEngine::ReddogLayer());
    }
};

Pagoda::Application* Pagoda::CreateApplication() {
    return new TestApp(std::string("TestApp"));
}