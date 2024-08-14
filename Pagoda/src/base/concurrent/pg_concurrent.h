#pragma once
#include "pgpch.h"

#ifdef PG_ARM64
#define THREAD_PAUSE(...) __yield();
#else
#define THREAD_PAUSE(...) _mm_pause();
#endif