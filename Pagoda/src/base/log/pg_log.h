#pragma once
#include "pgpch.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Pagoda::Base {
    class PAGODA_API Log {
    public:
        static void Init();
        static void SetClientName(std::string name);

        // Paste function at call, speeds up execution time.
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
            return s_CoreLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
            return s_ClientLogger;
        }

    private:
        // Loggers for both core and client.
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

#ifdef PG_DEBUG

#define PG_CORE_INFO(...) ::Pagoda::Base::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PG_CORE_TRACE(...) ::Pagoda::Base::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PG_CORE_WARNING(...) ::Pagoda::Base::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PG_CORE_ERROR(...) ::Pagoda::Base::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PG_CORE_CRITICAL(...) ::Pagoda::Base::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define PG_CORE_DEBUG(...) ::Pagoda::Base::Log::GetCoreLogger()->debug(__VA_ARGS__)

#define PG_INFO(...) ::Pagoda::Base::Log::GetClientLogger()->info(__VA_ARGS__)
#define PG_TRACE(...) ::Pagoda::Base::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PG_WARNING(...) ::Pagoda::Base::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PG_ERROR(...) ::Pagoda::Base::Log::GetClientLogger()->error(__VA_ARGS__)
#define PG_CRITICAL(...) ::Pagoda::Base::Log::GetClientLogger()->critical(__VA_ARGS__)

#else

#define PG_CORE_INFO
#define PG_CORE_TRACE
#define PG_CORE_WARNING
#define PG_CORE_ERROR
#define PG_CORE_CRITICAL
#define PG_CORE_DEBUG

#define PG_INFO
#define PG_TRACE
#define PG_WARNING
#define PG_ERROR
#define PG_CRITICAL

#endif

#ifdef PG_ENABLE_ASSERTS
#ifdef PG_PLATFORM_WINDOWS
#define PG_CORE_ASSERT(x, ...)                                   \
    {                                                            \
        if (!(x)) {                                              \
            PG_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); \
            __debugbreak();                                      \
        }                                                        \
    }
#define PG_CORE_ASSERT_CRITICAL(x, ...)                     \
    {                                                       \
        if (!(x)) {                                         \
            PG_CORE_CRITICAL("Critical: {0}", __VA_ARGS__); \
            __debugbreak();                                 \
        }                                                   \
    }
#define PG_ASSERT(x, ...)                                   \
    {                                                       \
        if (!(x)) {                                         \
            PG_ERROR("Assertion failed: {0}", __VA_ARGS__); \
            __debugbreak();                                 \
        }                                                   \
    }
#else
#define PG_CORE_ASSERT(x, ...)                                   \
    {                                                            \
        if (!x) {                                                \
            PG_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); \
            raise(SIGINT);                                       \
        }                                                        \
    }
#define PG_CORE_ASSERT_CRITICAL(x, ...)                     \
    {                                                       \
        if (!x) {                                           \
            PG_CORE_CRITICAL("Critical: {0}", __VA_ARGS__); \
            raise(SIGINT);                                  \
        }                                                   \
    }
#define PG_ASSERT(x, ...)                                   \
    {                                                       \
        if (!x) {                                           \
            PG_ERROR("Assertion failed: {0}", __VA_ARGS__); \
            raise(SIGINT);                                  \
        }                                                   \
    }

#endif
#else

#define PG_CORE_ASSERT(x, ...)
#define PG_CORE_ASSERT_CRITICAL(x, ...)
#define PG_ASSERT(x, ...)

#endif