#include <iostream>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "glut.h"
}

void lua(int argc, char* argv[])
{
    lua_State* L = lua_open();

    luaL_openlibs(L);

    lua_pushcfunction(L, luaopen_glut);
    lua_pushstring(L, "glut");
    lua_call(L, 1, 0);

    lua_newtable(L);
    for(int i = 0; i < argc; ++i)
    {
        lua_pushstring(L, argv[i]);
        lua_rawseti(L, -2, i - 1);
    }
    lua_setglobal(L, "arg");

    if(luaL_dofile(L, argv[1]) != 0)
    {
        std::cerr << "error : " << lua_tostring(L, -1) << std::endl;
    }

    lua_close(L);
}

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        std::cout << "usage: " << argv[0] << " <lua script file> [args...]" << std::endl;
        return 0;
    }

    lua(argc, argv);

    return 0;
}