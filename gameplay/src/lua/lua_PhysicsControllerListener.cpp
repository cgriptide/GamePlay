#include "Base.h"
#include "ScriptController.h"
#include "PhysicsController.h"
#include "lua_PhysicsControllerListener.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_PhysicsControllerListener()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"statusEvent", lua_PhysicsControllerListener_statusEvent},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("PhysicsController");

    sc->registerClass("PhysicsControllerListener", lua_members, NULL, NULL, lua_statics, scopePath);
}

static PhysicsController::Listener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsControllerListener");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsControllerListener' expected.");
    return (PhysicsController::Listener*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_PhysicsControllerListener_statusEvent(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                PhysicsController::Listener::EventType param1 = (PhysicsController::Listener::EventType)lua_enumFromString_PhysicsControllerListenerEventType(luaL_checkstring(state, 2));

                PhysicsController::Listener* instance = getInstance(state);
                instance->statusEvent(param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
