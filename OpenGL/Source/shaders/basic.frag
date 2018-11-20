#version 440 core

out vec4 color;

in vec3 Normal;
in vec3 FragPos;
in vec3 NowColor;
in vec3 NewLightPos;

void main(){

	vec3 lightColor = vec3(1.0f,1.0f,1.0f);

	//Diffuse
	vec3 norm = normalize(Normal);
//	vec3 lightDir = normalize(lightPos - FragPos);
vec3 lightDir = normalize(NewLightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff * lightColor;

//	vec3 result = diffuse * vec3(vertexColor);
	vec3 result = diffuse * vec3(NowColor);

	color = vec4(result,1.0f);
//	color = vertexColor;
//	color = vec4(NowColor,1.0f);
}