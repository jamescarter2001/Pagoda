#include "pgpch.h"

#include "pgLua.h"

namespace Pagoda::Lua {

    lua_State* LuaHandler::s_LuaState = nullptr;

    LuaHandler::LuaHandler() {
        if (!s_LuaState) {
            s_LuaState = luaL_newstate();
            PG_CORE_ASSERT(s_LuaState, "Error initializing Lua virtual machine.");
            PG_CORE_INFO("Lua initialization successful.");
        }
    }

    LuaHandler::~LuaHandler() {

    }

    LuaHandler* LuaHandler::Create() {
        return new LuaHandler;
    }
}