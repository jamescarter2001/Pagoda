#include "test_layer.h"

TestLayer::TestLayer(const std::string& name) : Layer(name) {}

TestLayer::~TestLayer() {

}

void TestLayer::OnEvent(Pagoda::Base::Event& e) const {
}

void TestLayer::OnUpdate() const {
}

void TestLayer::OnAttach() {
}