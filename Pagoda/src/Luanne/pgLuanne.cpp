#include "pgpch.h"

#include "pgLuanne.h"

namespace Pagoda::Luanne {

    lua_State* LuaHandler::s_LuaState = nullptr;

    LuaHandler::LuaHandler() {
        if (!s_LuaState) {
            s_LuaState = luaL_newstate();
            PG_CORE_ASSERT(s_LuaState, "Error initializing Lua virtual machine.");
        }
    }

    LuaHandler::~LuaHandler() {

    }

    LuaHandler* LuaHandler::Create() {
        return new LuaHandler;
    }
}