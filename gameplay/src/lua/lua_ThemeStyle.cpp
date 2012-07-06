#include "Base.h"
#include "ScriptController.h"
#include "ThemeStyle.h"
#include "lua_ThemeStyle.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_ThemeStyle()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"getTheme", lua_ThemeStyle_getTheme},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("Theme");

    sc->registerClass("ThemeStyle", lua_members, NULL, NULL, lua_statics, scopePath);
}

static Theme::Style* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ThemeStyle");
    luaL_argcheck(state, userdata != NULL, 1, "'ThemeStyle' expected.");
    return (Theme::Style*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_ThemeStyle_getTheme(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Theme::Style* instance = getInstance(state);
                void* returnPtr = (void*)instance->getTheme();
                if (returnPtr)
                {
                    ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Theme");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
