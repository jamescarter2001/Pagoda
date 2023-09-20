#pragma once

// DLL import/export macro, required for classes

#ifdef PG_PLATFORM_WINDOWS
#ifdef PG_BUILD_DLL
#define PAGODA_API __declspec(dllexport)
#elifdef PG_USE_DLL
#define PAGODA_API __declspec(dllimport)
#else
#define PAGODA_API
#endif
#else
#define PAGODA_API
#endif

// Bit field

#define BIT(x) (1 << x)

// Event function binding

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

// Numerical data types

#define PG_FLOAT 0
#define PG_UNSIGNED_INT 1
#define PG_UNSIGNED_BYTE 2

// Vector types

#define PG_VECTOR3 0
#define PG_VECTOR4 1