#pragma once
#include "pgpch.h"

#include "pg_core.h"

#include "base/log/pg_log.h"
#include "pg_application.h"

namespace Pagoda {
    extern Application* CreateApplication();
}

int main(int argc, char** argv) {
    Pagoda::Base::Log::Init();
    PG_CORE_INFO("Initialised log");

    auto app = Pagoda::CreateApplication();
    Pagoda::Base::Log::SetClientName(app->GetName());
    PG_CORE_DEBUG("Client name updated: {}", app->GetName());

    app->Run();
    delete app;
}