#include "pagoda.h"
#include "entry_point.h"

#include "test_layer.h"
#include "debug_layer.h"

class TestApp : public Pagoda::Application {
public:
    TestApp(const std::string& name)
        : Pagoda::Application(name) {
        PG_TRACE("Created new engine application");
        this->GetApplicationManager()->PushLayer(new TestLayer("TestLayer"));
        this->GetApplicationManager()->PushOverlay(new DebugLayer());
    }
};

Pagoda::Application* Pagoda::CreateApplication() {
    return new TestApp("TestApp");
}
