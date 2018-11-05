#version 440 core

uniform vec4 vertexColor; 
uniform vec3 lightPos;
out vec4 color;

in vec3 Normal;
in vec3 FragPos;

void main(){

	vec3 lightColor = vec3(1.0f,1.0f,1.0f);

	//Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = diffuse * vec3(vertexColor);

	color = vec4(result,1.0f);
//	color = vertexColor;
}