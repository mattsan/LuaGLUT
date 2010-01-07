extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <GLUT/glut.h>
#include <stdlib.h>

static lua_State *G_L;

static int interval = 200;

static int width  = 320;
static int height = 240;

static int vertex2i(lua_State *L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    glVertex2i(x, y);
    return 0;
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3d(0.0, 0.0, 0.0);
    glBegin(GL_LINES);

    lua_getfield(G_L, 1, "display");
    if(lua_isfunction(G_L, -1))
    {
        lua_pushvalue(G_L, 1);
        lua_call(G_L, 1, 0);
    }
    lua_settop(G_L, 1);

    glEnd();

    glFlush();
}

static void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width  = w;
    height = h;
    glLoadIdentity();
    glOrtho(-0.5, w - 0.5, h - 0.5, -0.5, -1.0, 1.0);
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

    glColor3d(0.0, 0.0, 0.0);
    glBegin(GL_LINES);

    lua_getfield(G_L, 1, "display");
    if(lua_isfunction(G_L, -1))
    {
        lua_pushvalue(G_L, 1);
        lua_call(G_L, 1, 0);
    }
    lua_settop(G_L, 1);

    glEnd();

    glFlush();

    glutTimerFunc(interval, timer, 0);
}

static int main_loop(lua_State *L)
{
    int   argc = 0;
    char* argv[] = { 0 };

    G_L = L;

    lua_getfield(G_L, 1, "width");
    if(lua_isnumber(G_L, -1))
    {
        width = lua_tointeger(G_L, -1);
    }
    lua_getfield(G_L, 1, "height");
    if(lua_isnumber(G_L, -1))
    {
        height = lua_tointeger(G_L, -1);
    }
    lua_getfield(G_L, 1, "interval");
    if(lua_isnumber(G_L, -1))
    {
        interval = lua_tointeger(G_L, -1);
    }
    lua_settop(G_L, 1);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(width, height);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);

    glutCreateWindow(argv[0]);

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(interval, timer, 0);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutMainLoop();

    return 0;
}

static const luaL_Reg glutlib[] =
{
    { "vertex2i",  vertex2i   },
    { "main_loop", main_loop  },
    { NULL,        NULL       }
};

extern "C"
{

LUALIB_API int luaopen_glut(lua_State *L)
{
    luaL_register(L, "glut", glutlib);
    return 1;
}

} // extern "C"
