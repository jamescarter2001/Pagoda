#pragma once

#include <Pagoda.h>

class DebugLayer : public Pagoda::Universe::Layer {
public:
    DebugLayer();
    virtual ~DebugLayer();

    virtual void OnEvent(Pagoda::Universe::ApplicationContext& ctx, Pagoda::Base::Event& e) const override;
    virtual void OnUpdate(Pagoda::Universe::ApplicationContext& ctx) override;
    virtual void OnAttach(Pagoda::Universe::ApplicationContext& ctx) override;

private:
    bool show_demo_window = false;
    bool show_another_window = true;
};
