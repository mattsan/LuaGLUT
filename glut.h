#ifndef GLUT_H
#define GLUT_H

extern "C"
{
#include <lua.h>
}

LUALIB_API int luaopen_glut(lua_State* L);

#endif//GLUT_H
