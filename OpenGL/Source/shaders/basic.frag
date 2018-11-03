#version 440 core

uniform vec4 vertexColor; 
out vec4 color;

void main(){
	color = vertexColor;
}