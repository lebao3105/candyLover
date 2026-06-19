#include "dsl/dsl.h"
#include <string.h>
#include <lauxlib.h>
#include <logmod.h>

struct lua_State *Lua = nullptr;

int include(struct lua_State *L)
{
    int result = 0; // 0 = fail, 1 = success

    WARN_TOO_MANY_ARGS(1, include);
    else if (lua_gettop(L) == 0)
    {
        luaL_error(L, "include() needs ONE argument!", nullptr);
        goto done;
    }

    char *path = luaL_checkstring(L, 1);
    if (path == nullptr || strlen(path) == 0)
    {
        luaL_error(L, "Empty path", nullptr);
        goto done;
    }

    DSL_load_file(path);
    goto done;

done:
    lua_pushboolean(L, result);
    return 1;
}

void DSL_global_init(lua_State *L)
{
    REGISTER_C_FUNCTION(include);
}

void DSL_load_file(const char *path)
{
    logmod_log(INFO, nullptr, "Bắt đầu đọc %s\n", path);
    if (luaL_loadfile(Lua, path) == LUA_OK)
    {
        if (lua_pcall(Lua, 0, LUA_MULTRET, 0) == LUA_OK)
        {
            logmod_log(INFO, nullptr, "Đọc %s thành công\n", path);
            return;
        }
        else
            logmod_log(ERROR, nullptr, "Lỗi chạy file %s: %s\n", path, lua_tostring(Lua, -1));
    }
    else
        logmod_log(ERROR, nullptr, "Nạp %s thất bại: %s\n", path, lua_tostring(Lua, -1));
    lua_pop(Lua, 1);
}
