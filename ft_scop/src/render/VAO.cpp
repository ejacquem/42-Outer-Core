#include"VAO.hpp"

VAO::VAO()
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}

void VAO::bind()
{
	glBindVertexArray(id);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

void VAO::deleteVAO()
{
	glDeleteVertexArrays(1, &id);
}