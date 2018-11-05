#include <iostream>

#include "Source/model/model.h"
#include "Source/utilities/objutils.h"
#include "Source/graphics/Window.h"
#include "Source/graphics/shader.h"
#include "Source/graphics/buffer/indexbuffer.h"
#include "Source/graphics/buffer/vertexarray.h"
#include "Source/graphics/camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define Window_TITLE "ZJU GAME DEVELOPEMENT"


using namespace std;

struct Node {
	GLuint v_num;
	GLuint n_num;
	Node(int v, int n) :v_num(v), n_num(n) {};
};
vector <Node> list;

int main() {

	Model model = ObjUtils::readObj("Source/model/dragon.obj");
	vector <vec3> vertexes;
	vector <vec3> normals;

//	vertexes.resize(model.m_vertexIndices.size() * 3);
//	normals.resize(model.m_normalIndices.size() * 3);

	//vertexes.resize(36);
	//normals.resize(36);

	for (int i = 0; i < model.m_vertexIndices.size(); i++) {
		for (int j = 0; j < 3; j++) {
			int v_index = model.m_vertexIndices[i].index[j];
			vertexes.push_back(model.m_vertexes[v_index]);
		}
	}
	


	int count = 0;
	for (int i = 0; i < model.m_vertexIndices.size(); i++) {
		for (int j = 0; j < 3; j++) {
			int n_index = model.m_normalIndices[i].index[j];
			normals.push_back(model.m_normals[n_index]);
		}
	}



	//for (int i = 0; i < 36; i++) {
	//	scanf("%f %f %f %f %f %f", &vertexes[i].x, &vertexes[i].y, &vertexes[i].z,
	//		&normals[i].x, &normals[i].y, &normals[i].z);
	//}



	Window window = Window(Window_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	VertexArray vao;
	vao.addBuffer(new Buffer(&vertexes[0].x, vertexes.size() * 3, 3), 0);
	vao.addBuffer(new Buffer(&normals[0].x, normals.size() * 3, 3), 1);



	//	vao.addBuffer(new Buffer(&model.m_vertexes[0].x, model.m_vertexes.size() * 3, 3), 0);
	//	IndexBuffer ibo(&model.m_vertexIndices[0].index[0], model.m_vertexIndices.size() * 3);

	Shader shader("Source/shaders/basic.vert", "Source/shaders/basic.frag");
	shader.enable();

	mat4 scala_matric = mat4::scale(vec3(0.5, 0.5, 0.5));

	GLfloat xoffet = 0, yoffset = -5, zoffet = -15;
	//GLfloat xoffet = 0, yoffset = 0, zoffet = -5;
	GLfloat yaw = 0, ditch = 0;

	shader.setUniformMat4("scale_matrix", mat4::scale(vec3(0.1, 0.1, 0.1)));
	shader.setUniform4f("vertexColor", vec4(0.2f, 0.3f, 0.8f, 1.0f));
	shader.setUniformMat4("perspective_matrix", mat4::perspective(45, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f));
	shader.setUniformMat4("translation_matrix", mat4::translation(vec3(xoffet, yoffset, zoffet)));
	shader.setUniformMat4("rotation_matrix", mat4::rotation(yaw, vec3(0, 1, 0)));
	shader.setUniformMat4("rotation_matrix", mat4::rotation(ditch, vec3(1, 0, 0)));
	shader.setUniform3f("lightPos", vec3(1.2f, 1.0f, 2.0f));
	
	glEnable(GL_DEPTH_TEST);

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat camerSpeed;

	while (!window.closed()) {


		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camerSpeed = 5 * deltaTime;

		if (window.isKeyPressed(GLFW_KEY_W)) {
			zoffet += camerSpeed;
		}
		if (window.isKeyPressed(GLFW_KEY_S)) {
			zoffet -= camerSpeed;
		}
		if (window.isKeyPressed(GLFW_KEY_A) || window.isKeyPressed(GLFW_KEY_LEFT)) {
			xoffet += camerSpeed;
		}
		if (window.isKeyPressed(GLFW_KEY_D) || window.isKeyPressed(GLFW_KEY_RIGHT)) {
			xoffet -= camerSpeed;
		}
		if (window.isKeyPressed(GLFW_KEY_UP)) {
			yoffset += camerSpeed;
		}
		if (window.isKeyPressed(GLFW_KEY_DOWN)) {
			yoffset -= camerSpeed;
		}


		if (window.isKeyPressed(GLFW_KEY_Q)) {
			yaw += camerSpeed * 3;
			shader.setUniformMat4("rotation_matrix", mat4::rotation(yaw, vec3(0, 1, 0)));
		}
		if (window.isKeyPressed(GLFW_KEY_E)) {
			yaw -= camerSpeed * 3;
			shader.setUniformMat4("rotation_matrix", mat4::rotation(yaw, vec3(0, 1, 0)));
		}




		shader.setUniformMat4("translation_matrix", mat4::translation(vec3(xoffet, yoffset, zoffet)));



		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		window.clear();

		vao.bind();
		//		ibo.bind();
		//		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertexes.size());
		//		ibo.unbind();
		vao.unbind();

		window.update();
	}
	shader.disable();

	return 0;
}