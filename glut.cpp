extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

#include <GLUT/glut.h>

#include "glut.h"

namespace
{

enum
{
    STROKE_ROMAN,
    STROKE_MONO_ROMAN
};

lua_State* G_L;

// prefix `gl' functions

int begin(lua_State* L)
{
    GLenum mode = luaL_checkinteger(L, 1);
    glBegin(mode);
    return 0;
}

int clear(lua_State* L)
{
    GLbitfield mask = luaL_checkinteger(L, 1);
    glClear(mask);
    return 0;
}

int clear_color(lua_State* L)
{
    GLclampf red   = luaL_checknumber(L, 1);
    GLclampf green = luaL_checknumber(L, 2);
    GLclampf blue  = luaL_checknumber(L, 3);
    GLclampf alpha = luaL_checknumber(L, 4);
    glClearColor(red, green, blue, alpha);
    return 0;
}

int color3dv(lua_State* L)
{
    GLdouble rgb[3];
    rgb[0] = luaL_checknumber(L, 1);
    rgb[1] = luaL_checknumber(L, 2);
    rgb[2] = luaL_checknumber(L, 3);
    glColor3dv(rgb);
    return 0;
}

int end(lua_State* L)
{
    glEnd();
    return 0;
}

int load_identity(lua_State* L)
{
    glLoadIdentity();
    return 0;
}

int ortho(lua_State* L)
{
    GLdouble left   = luaL_checknumber(L, 1);
    GLdouble right  = luaL_checknumber(L, 2);
    GLdouble bottom = luaL_checknumber(L, 3);
    GLdouble top    = luaL_checknumber(L, 4);
    GLdouble zNear  = luaL_checknumber(L, 5);
    GLdouble zFar   = luaL_checknumber(L, 6);
    glOrtho(left, right, bottom, top, zNear, zFar);
    return 0;
}

int pop_matrix(lua_State* L)
{
    glPopMatrix();
    return 0;
}

int push_matrix(lua_State* L)
{
    glPushMatrix();
    return 0;
}

int scalef(lua_State* L)
{
    GLfloat x = luaL_checknumber(L, 1);
    GLfloat y = luaL_checknumber(L, 2);
    GLfloat z = luaL_checknumber(L, 3);
    glScalef(x, y, z);
    return 0;
}

int translatef(lua_State* L)
{
    GLfloat x = luaL_checknumber(L, 1);
    GLfloat y = luaL_checknumber(L, 2);
    GLfloat z = luaL_checknumber(L, 3);
    glTranslatef(x, y, z);
    return 0;
}

int vertex2i(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    glVertex2i(x, y);
    return 0;
}

int viewport(lua_State* L)
{
    GLint x        = luaL_checkinteger(L, 1);
    GLint y        = luaL_checkinteger(L, 2);
    GLsizei width  = luaL_checkinteger(L, 3);
    GLsizei height = luaL_checkinteger(L, 4);
    glViewport(x, y, width, height);
    return 0;
}

// prefix `glut' functions

int create_window(lua_State* L)
{
    const char* title = luaL_checkstring(L, 1);
    lua_pushinteger(L, glutCreateWindow(title));
    return 1;
}

int init(lua_State* L)
{
    int     argc = lua_gettop(L);
    char**  argv = new char*[argc];
    for(int i = 0; i < argc; ++i)
    {
        argv[i] = const_cast<char*>(luaL_checkstring(L, i + 1));
    }
    glutInit(&argc, argv);
    delete[] argv;
    return 0;
}

int init_display_mode(lua_State* L)
{
    unsigned int mode = static_cast<unsigned int>(luaL_checkinteger(L, 1));
    glutInitDisplayMode(mode);
    return 0;
}

int init_window_position(lua_State* L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    glutInitWindowPosition(x, y);
    return 0;
}

int init_window_size(lua_State* L)
{
    int width  = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);
    glutInitWindowSize(width, height);
    return 0;
}
void* getFont(int font_id)
{
    switch(font_id)
    {
    case STROKE_ROMAN:      return GLUT_STROKE_ROMAN;
    case STROKE_MONO_ROMAN: return GLUT_STROKE_MONO_ROMAN;
    default:                return 0;
    }
}

int stroke_character(lua_State* L)
{
    void* font      = getFont(luaL_checkinteger(L, 1));
    int   character = luaL_checkinteger(L, 2);
    if(font != 0)
    {
        glutStrokeCharacter(font, character);
    }
    return 0;
}

int swap_buffers(lua_State* L)
{
    glutSwapBuffers();
    return 0;
}

void display()
{
    lua_getfield(G_L, 1, "display");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_call(G_L, 1, 0);
}

void reshape(int w, int h)
{
    lua_getfield(G_L, 1, "reshape");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_pushinteger(G_L, w);
    lua_pushinteger(G_L, h);
    lua_call(G_L, 3, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    lua_getfield(G_L, 1, "keyboard");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_pushinteger(G_L, key);
    lua_pushinteger(G_L, x);
    lua_pushinteger(G_L, y);
    lua_call(G_L, 4, 0);
}

void special(int key, int x, int y)
{
    lua_getfield(G_L, 1, "special");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_pushinteger(G_L, key);
    lua_pushinteger(G_L, x);
    lua_pushinteger(G_L, y);
    lua_call(G_L, 4, 0);
}

void timer(int n)
{
    lua_getfield(G_L, 1, "timer");
    if( ! lua_isfunction(G_L, -1))
    {
        lua_pop(G_L, 1);
        return;
    }

    lua_pushvalue(G_L, 1);
    lua_pushinteger(G_L, n);
    lua_call(G_L, 1, 0);
    lua_getfield(G_L, 1, "interval");
    unsigned int interval = static_cast<unsigned int>(luaL_checkinteger(G_L, -1));
    glutTimerFunc(interval, timer, 0);
    lua_pop(G_L, 1);
}

int main_loop(lua_State* L)
{
    G_L = L;

    lua_getfield(L, 1, "display");
    if(lua_isfunction(L, -1))
    {
        glutDisplayFunc(display);
    }

    lua_getfield(L, 1, "keyboard");
    if(lua_isfunction(L, -1))
    {
        glutKeyboardFunc(keyboard);
    }

    lua_getfield(L, 1, "reshape");
    if(lua_isfunction(L, -1))
    {
        glutReshapeFunc(reshape);
    }

    lua_getfield(L, 1, "special");
    if(lua_isfunction(L, -1))
    {
        glutSpecialFunc(special);
    }

    lua_getfield(L, 1, "timer");
    if(lua_isfunction(L, -1))
    {
        lua_getfield(L, 1, "interval");
        unsigned int interval = static_cast<unsigned int>(luaL_checkinteger(L, -1));
        glutTimerFunc(interval, timer, 0);
    }
    lua_settop(L, 1);

    glutMainLoop();

    return 0;
}

const luaL_Reg gllib[] =
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

const luaL_Reg glutlib[] =
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

} // anonymous namespace

LUALIB_API int luaopen_glut(lua_State* L)
{
    luaL_register(L, "gl", gllib);
    luaL_register(L, "glut", glutlib);
    return 1;
}
