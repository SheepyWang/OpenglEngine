#include "buffer.h"

Buffer::Buffer(GLfloat *data, GLsizei count, GLuint componentCount) 
	:m_ComponentCount(componentCount){
	glGenBuffers(1, &m_BufferID);//���ID
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);//��ID
	glBufferData(GL_ARRAY_BUFFER, count * sizeof(GL_FLOAT), data, GL_STATIC_DRAW);//��������
	glBindBuffer(GL_ARRAY_BUFFER, 0);//���
}

Buffer::~Buffer(){
	glDeleteBuffers(1,&m_BufferID);
}

void Buffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
}

void Buffer::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint Buffer::getComponentCount() const {
	return m_ComponentCount;
}