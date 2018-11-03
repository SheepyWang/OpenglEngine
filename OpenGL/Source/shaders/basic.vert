#version 440 core

layout (location = 0) in vec4 position;

uniform mat4 scale_matrix = mat4(1.0);
uniform mat4 view_matrix = mat4(1.0);
uniform mat4 perspective_matrix = mat4(1.0);

void main(){
	gl_Position = perspective_matrix * view_matrix * scale_matrix * position;
//	gl_Position = view_matrix * scale_matrix * position;
}