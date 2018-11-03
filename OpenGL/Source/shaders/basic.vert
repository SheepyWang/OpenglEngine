#version 440 core

layout (location = 0) in vec4 position;

uniform mat4 scale_matrix = mat4(1.0);

void main(){
	gl_Position = scale_matrix * position;
}