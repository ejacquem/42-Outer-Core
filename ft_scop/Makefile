NAME	= build/gl
FLAGS	= -g -Wall -Wextra -std=c++17 -Wshadow -Wno-shadow
GLAD	= external/glad/src/glad.c
GLADO	= build/glad.o
GLFWLIB	= external/glfw-3.4/build/src/libglfw3.a
LIBS	= ${GLFWLIB} -lm -ldl -lpthread -lX11 -lGL -lXrandr -lXi
INCLUDE	= -I./include -I./include/render -I./external

GALLIUM = GALLIUM_HUD_PERIOD=0.05 GALLIUM_HUD=".dfps"

SRC		= src/gl.cpp src/render/Shader.cpp src/Time.cpp\
			src/render/VBO.cpp\
			src/render/VAO.cpp\
			src/render/EBO.cpp\
			src/LineDrawer.cpp\
			src/Object.cpp\
			src/ObjectLoader.cpp

all: ${NAME}

go: all
	${GALLIUM} ./${NAME}

${GLADO}: ${GLAD}
	gcc -Wall -Iexternal/glad/include -c ${GLAD} -o ${GLADO}

${NAME}: ${SRC} ${GLADO}
	c++ ${INCLUDE} ${FLAGS} -o ${NAME} ${SRC} ${GLADO} ${LIBS}

clean:
	rm -f ${NAME} ${GLADO}

fclean: clean

re: fclean all
