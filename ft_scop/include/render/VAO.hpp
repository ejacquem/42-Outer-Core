#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include "glad/include/glad/glad.h"
#include "VBO.hpp"

//Vertex Array Object
class VAO
{
public:
	GLuint id;
	VAO();

	void bind();
	static void unbind();
	void deleteVAO();
};
#endif