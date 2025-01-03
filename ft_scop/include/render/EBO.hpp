#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include "glad/include/glad/glad.h"

//Element Buffer Object
class EBO
{
public:
	GLuint id;
	EBO(GLuint* indices, GLsizeiptr size);

	void bind();
	static void unbind();
	void deleteEBO();
};

#endif