extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <GLUT/glut.h>

#include "glut.h"

static lua_State *G_L;

static int begin(lua_State *L)
{
glBegin
}

static int clear(lua_State *L)
{
glClear
}

static int clear_color(lua_State *L)
{
glClearColor
}

static int color3dv(lua_State *L)
{
glColor3dv
}

static int end(lua_State *L)
{
glEnd
}

static int load_identity(lua_State *L)
{
glLoadIdentity
}

static int ortho(lua_State *L)
{
glOrtho
}

static int pop_matrix(lua_State *L)
{
glPopMatrix
}

static int push_matrix(lua_State *L)
{
glPushMatrix
}

static int scalef(lua_State *L)
{
glScalef
}

static int translatef(lua_State *L)
{
glTranslatef
}

static int vertex2i(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    glVertex2i(x, y);
    return 0;
}

static int viewport(lua_State *L)
{
glViewport
}

static const luaL_Reg gllib[] =
{
    { "begin",         begin         },
    { "clear",         clear         },
    { "clear_color",   clear_color   },
    { "color3dv",      color3dv      },
    { "end",           end           },
    { "load_identity", load_identity },
    { "ortho",         ortho         },
    { "pop_matrix",    pop_matrix    },
    { "push_matrix",   push_matrix   },
    { "scalef",        scalef        },
    { "translatef",    translatef    },
    { "vertex2i",      vertex2i      },
    { NULL,            NULL          }
};

extern "C"
{

LUALIB_API int luaopen_gl(lua_State* L)
{
    luaL_register(L, "gl", gllib);
    return 1;
}

} // extern "C"

static int create_window(lua_State* L)
{
glutCreateWindow
}

static int init(lua_State* L)
{
glutInit
}

static int init_display_mode(lua_State* L)
{
glutInitDisplayMode
}

static int init_window_position(lua_State* L)
{
glutInitWindowPosition
}

static int init_window_size(lua_State* L)
{
glutInitWindowSize
}

static int stroke_character(lua_State* L)
{
glutStrokeCharacter
}

static int swap_buffers(lua_State* L)
{
glutSwapBuffers
}

static void display()
{
    lua_getfield(G_L, 1, "display");
    if(lua_isfunction(G_L, -1))
    {
        lua_pushvalue(G_L, 1);
        lua_call(G_L, 1, 0);
    }
    lua_settop(G_L, 1);
}

static void reshape(int w, int h)
{
    lua_getfield(G_L, 1, "reshape");
    if(lua_isfunction(G_L, -1))
    {
        lua_pushvalue(G_L, 1);
        lua_call(G_L, 1, 0);
    }
    lua_settop(G_L, 1);
}

static void keyboard(unsigned char key, int x, int y)
{
    lua_getfield(G_L, 1, "keyboard");
    if(lua_isfunction(G_L, -1))
    {
        lua_pushvalue(G_L, 1);
        lua_pushinteger(G_L, key);
        lua_pushinteger(G_L, x);
        lua_pushinteger(G_L, y);
        lua_call(G_L, 4, 0);
    }
    lua_settop(G_L, 1);
}

static void timer(int n)
{
    lua_getfield(G_L, 1, "timer");
    if(lua_isfunction(G_L, -1))
    {
        lua_pushvalue(G_L, 1);
        lua_call(G_L, 1, 0);
    }
    lua_settop(G_L, 1);
    glutTimerFunc(interval, timer, 0);
}

static int main_loop(lua_State* L)
{
    G_L = L;

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutTimerFunc(timer);

    glutMainLoop();

    return 0;
}

static const luaL_Reg glutlib[] =
{
    { "create_window",        create_window        },
    { "init_display_mode",    init_display_mode    },
    { "init_window_position", init_window_position },
    { "init_window_size",     init_window_size     },
    { "stroke_character",     stroke_character     },
    { "swap_buffers",         swap_buffers         },
    { "main_loop",            main_loop            },
    { NULL,                   NULL                 }
};

extern "C"
{

LUALIB_API int luaopen_glut(lua_State* L)
{
    luaL_register(L, "glut", glutlib);
    return 1;
}

} // extern "C"
