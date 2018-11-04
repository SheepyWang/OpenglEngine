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

	
	shader.setUniformMat4("scale_matrix", mat4::scale(vec3(0.1,0.1,0.1)));
	shader.setUniform4f("vertexColor", vec4(0.2f, 0.3f, 0.8f, 1.0f));


	//摄像机方向
	vec3 cameraPos = vec3(0.0, 0.0f, 3.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
	vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);

	
	GLfloat cameraSpeed = 0.01;

	mat4 view;

	//通过时间控制速度，统一速度
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	//鼠标坐标
	GLfloat lastX = WINDOW_WIDTH / 2;
	GLfloat lastY = WINDOW_HEIGHT / 2;
	double xpos = WINDOW_WIDTH / 2;
	double ypos = WINDOW_HEIGHT / 2;
	GLfloat yaw = -90;
	GLfloat pitch = 0;
	double aspect;

	bool isFirstMouse = true;




	while (!window.closed()) {

	

		//速度同步
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		cameraSpeed = 4 * deltaTime;

		//键盘处理
		if (window.isKeyPressed(GLFW_KEY_W)) {
			cameraPos += cameraFront * cameraSpeed;
		}
		if (window.isKeyPressed(GLFW_KEY_S)) {
			cameraPos -= cameraFront * cameraSpeed;
		}
		if (window.isKeyPressed(GLFW_KEY_A)) {
			cameraPos -= vec3::normalize(vec3::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (window.isKeyPressed(GLFW_KEY_D)) {
			cameraPos += vec3::normalize(vec3::cross(cameraFront, cameraUp)) * cameraSpeed;
		}

		if (window.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
			window.getMousePosition(xpos, ypos);
		}
		else{
			isFirstMouse = true;
		}

		window.getAsepct(aspect);
		cout << aspect << endl;
		shader.setUniformMat4("perspective_matrix", mat4::perspective(aspect, (GLfloat)WINDOW_HEIGHT / (GLfloat)WINDOW_WIDTH, 0.1f, 100.0f));
			   
		//鼠标处理
		if (isFirstMouse) {
			lastX = xpos;
			lastY = ypos;
			isFirstMouse = false;
		}

		GLfloat xoffet = xpos - lastX;
		GLfloat yoffet = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		GLfloat sensitivity = 0.05;
		xoffet *= -sensitivity;
		yoffet *= -sensitivity;

		yaw += xoffet;
		pitch += yoffet;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}
		
		vec3 front;
		front.y = sin(glm::radians(pitch));
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = vec3::normalize(front);
		

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