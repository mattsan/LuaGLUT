# for Mac OS X

lua_glut: lua_glut.cpp glut.cpp glut.h
	g++ -framework OpenGL -framework GLUT -framework Foundation -ansi -Wall -o lua_glut lua_glut.cpp glut.cpp -llua