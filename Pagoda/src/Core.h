#pragma once

// DLL import/export macro, required for classes.

#ifdef PG_PLATFORM_WINDOWS
#ifdef PG_BUILD_DLL
#define PAGODA_API __declspec(dllexport)
#elifdef PG_USE_DLL
#define PAGODA_API __declspec(dllimport)
#else
#define PAGODA_API
#endif
#else
#error Unsupported platform
#endif

#ifdef PG_ENABLE_ASSERTS

#define PG_CORE_ASSERT(x, ...) {if (!x) { CF_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();} }
#define PG_ASSERT(x, ...) {if (!x) { CF_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();} }

#else

#define PG_CORE_ASSERT(x, ...)
#define PG_ASSERT(x, ...)

#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)