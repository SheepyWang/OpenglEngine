#pragma once

#ifndef _VERTEX_ARRAY_H_
#define _VERTEX_ARRAY_H_

#include <iostream>
#include <vector>

#include "buffer.h"

class VertexArray {
private:
	GLuint m_ArrayID;
	std::vector<Buffer *> m_Buffers;

public:
	VertexArray();
	~VertexArray();
	void addBuffer(Buffer * buffer, GLuint index);
	void bind() const;
	void unbind() const;
};

#endif