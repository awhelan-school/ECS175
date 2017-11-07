CC = /usr/bin/g++ -std=c++11 

#OPENGL LIBS FOR WIN
#GLLIB := -L/usr/lib/ -lopengl32 -lglu32 -lfreeglut
#OPENGL LIBS FOR LINUX
GLLIB :=  -lGL -lGLEW -lGLU -lglut
#OPENGL LIBS FOR MAC
#GLLIB := -framework OpenGL -framework GLUT

#COMPILER FLAGS
CCFLAGS :=

#include directories
#should include gl.h glut.h etc...
SRC_DIR    := ./src
HEADERS    := -I./src/include/
INCDIR	   := -I/usr/include
LDLIBS	   := $(GLLIB)

TARGET = glutdemo
OBJS = demo.o data.o fileIO.o Matrix.o


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(CCFLAGS) $(LDLIBS)

%.o : $(SRC_DIR)/%.cpp
	$(CC) $(CCFLAGS) $(HEADERS) $(INCDIR)  -c $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET) 

