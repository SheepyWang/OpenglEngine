#include <iostream>

#include "Source/model/model.h"
#include "Source/utilities/objutils.h"
#include "Source/graphics/Window.h"
#include "Source/graphics/shader.h"
#include "Source/graphics/buffer/indexbuffer.h"
#include "Source/graphics/buffer/vertexarray.h"
#include "Source/graphics/camera.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define _USE_MATH_DEFINES
#include <math.h>


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define Window_TITLE "ZJU GAME DEVELOPEMENT"

using namespace std;

int main() {

	Model model = ObjUtils::readObj("Source/model/dragon.obj");

	Window window = Window(Window_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	VertexArray vao;
	vao.addBuffer(new Buffer(&model.m_vertexes[0].x, model.m_vertexes.size() * 3, 3), 0);
	IndexBuffer ibo(&model.m_vertexIndices[0].index[0], model.m_vertexIndices.size() * 3);

	Shader shader("Source/shaders/basic.vert", "Source/shaders/basic.frag");
	shader.enable();

	mat4 scala_matric = mat4::scale(vec3(0.5, 0.5, 0.5));

	shader.setUniformMat4("perspective_matrix", mat4::perspective(45, (GLfloat)WINDOW_HEIGHT / (GLfloat)WINDOW_WIDTH, 0.1f, 100.0f));
	shader.setUniformMat4("scale_matrix", mat4::scale(vec3(0.1,0.1,0.1)));
	shader.setUniform4f("vertexColor", vec4(0.2f, 0.3f, 0.8f, 1.0f));


	//ÉãÏñ»ú·½Ïò
	vec3 cameraPos = vec3(0.0, 0.0f, 3.0f);
	vec3 cameraTarget = vec3(0.0f, 0.0f, 0.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);

	glm::vec3 cameraPos2 = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront2 = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp2 = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view2 = glm::lookAt(cameraPos2, cameraPos2 + cameraFront2, cameraUp2);
	
	GLfloat cameraSpeed = 0.01;

	mat4 view;

	view = mat4(3);
	for (int i = 0; i < 4; i++) {
		cout << view.columns[i];
	}

	bool isPrint = false;

	while (!window.closed()) {

		

		if (window.isKeyPressed(GLFW_KEY_W)) {
			cameraPos += cameraFront * cameraSpeed;
			cameraPos2 += cameraFront2 * cameraSpeed;
			isPrint = true;
		}
		if (window.isKeyPressed(GLFW_KEY_S)) {
			cameraPos -= cameraFront * cameraSpeed;
			cameraPos2 -= cameraFront2 * cameraSpeed;
			isPrint = true;
		}
		if (window.isKeyPressed(GLFW_KEY_A)) {
			cameraPos -= vec3::normalize(vec3::cross(cameraFront, cameraUp)) * cameraSpeed;
			cameraPos2 -= glm::normalize(glm::cross(cameraFront2, cameraUp2)) * cameraSpeed;
			isPrint = true;
		}
		if (window.isKeyPressed(GLFW_KEY_D)) {
			cameraPos += vec3::normalize(vec3::cross(cameraFront, cameraUp)) * cameraSpeed;
			cameraPos2 += glm::normalize(glm::cross(cameraFront2, cameraUp2)) * cameraSpeed;
			isPrint = true;
		}
		if (isPrint) {

			cout << "canPos :" << cameraPos << endl;
			cout << "canPos2 :";
			for (int i = 0; i < 3; i++) {
				cout << cameraPos2[i] << " ";
			}
			cout << endl;

			cout << "view: " << endl;
			for (int i = 0; i < 4; i++) {
				cout << view.columns[i];
			}
			cout << endl;

			cout << "view2: " << endl;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					cout << view2[i][j] << " ";
				}
				cout << endl;
			}
			cout << endl;
			isPrint = false;
		}

		view2 = glm::lookAt(cameraPos2, cameraPos2 + cameraFront2, cameraUp2);
		view = Camer::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
		shader.setUniformMat4("view_matrix", view);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		window.clear();

		vao.bind();
		ibo.bind();
		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
		ibo.unbind();
		vao.unbind();

		window.update();
	}
	shader.disable();

	return 0;
}