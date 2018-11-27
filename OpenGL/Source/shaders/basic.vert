#version 440 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 colors;

uniform mat4 perspective_matrix = mat4(1.0);
uniform mat4 view_matrix = mat4(1.0);
uniform vec3 lightPos;
uniform mat4 model_matrix = mat4(1.0);


out vec3 Normal;
out vec3 FragPos;
out vec3 NowColor;
out vec3 NewLightPos;

void main(){
//	gl_Position = perspective_matrix * scale_matrix * rotation_matrix * translation_matrix * position;
	Normal = vec3(normal);
	gl_Position = perspective_matrix * view_matrix * model_matrix * position;
	NowColor = vec3(colors);
//	FragPos = vec3(translation_matrix * rotation_matrix *position);
	FragPos = vec3(model_matrix * position);
	NewLightPos = vec3(view_matrix * vec4(lightPos,1.0f));
}