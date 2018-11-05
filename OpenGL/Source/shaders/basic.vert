#version 440 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;

uniform mat4 scale_matrix = mat4(1.0);
uniform mat4 perspective_matrix = mat4(1.0);
uniform mat4 translation_matrix = mat4(1.0);
uniform mat4 rotation_matrix = mat4(1.0);

out vec3 Normal;
out vec3 FragPos;

void main(){
	gl_Position = perspective_matrix * scale_matrix * translation_matrix * rotation_matrix * position;
	Normal = vec3(normal);
	FragPos = vec3(translation_matrix * rotation_matrix *position);
}