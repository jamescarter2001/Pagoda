#include "pgpch.h"
#include "pg_log.h"

namespace Pagoda::Base {
    // Bring into scope from header file.
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

    void Log::Init() {
        // Format logger output.
        spdlog::set_pattern("%^[%T] %n: %v%$");

        // initialize loggers with names.
        Log::s_CoreLogger = spdlog::stdout_color_mt("PAGODA");
        // Output every log type.
        s_CoreLogger->set_level(spdlog::level::trace);

        Log::s_ClientLogger = spdlog::stdout_color_mt("CLIENT");
        s_ClientLogger->set_level(spdlog::level::trace);
    }

    void Log::SetClientName(std::string name) {
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        Log::s_ClientLogger = spdlog::stdout_color_mt(name);
    }
}