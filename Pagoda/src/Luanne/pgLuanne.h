#pragma once
#include "pgpch.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace Pagoda::Luanne {
    class PAGODA_API LuaHandler {
    public:
        static lua_State* s_LuaState;

        LuaHandler();
        virtual ~LuaHandler();

        static LuaHandler* Create();
    };
}