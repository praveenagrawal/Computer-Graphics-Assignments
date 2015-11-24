CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw3
GLUTLIB = -lglut
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB) $(GLUTLIB)
LDFLAGS=-L/usr/local/lib `pkg-config --static --libs glfw3`
CPPFLAGS=-I/usr/local/include

BIN=main
SRCS=main.cpp gl_framework.cpp shader_util.cpp file_util.cpp hierarchy_node.cpp texture.cpp
INCLUDES=gl_framework.hpp shader_util.hpp main.hpp file_util.hpp hierarchy_node.hpp Light.hpp texture.hpp

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS)

clean:
	rm -f *~ *.o $(BIN)
