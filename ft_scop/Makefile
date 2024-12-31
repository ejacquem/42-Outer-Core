NAME=gl
FLAGS= -g -Wall -Wextra -Werror -std=c++17 -Wshadow -Wno-shadow
LIBS= glfw-3.4/build/src/libglfw3.a -lm -ldl -lpthread -lX11 -lGL -lXrandr -lXi

SRC= gl.cpp Shader.cpp

all: ${NAME}

go: all
	GALLIUM_HUD_PERIOD=0.05 GALLIUM_HUD=".dfps" ./${NAME}

glad.o: glad/src/glad.c
	gcc -Wall -Iglad/include -c glad/src/glad.c -o glad.o

${NAME}: ${SRC} glad.o
	c++ ${FLAGS} -o ${NAME} ${SRC} glad.o ${LIBS}

clean:
	rm -f ${NAME} glad.o

fclean: clean

re: fclean all