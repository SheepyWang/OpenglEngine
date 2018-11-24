#include <iostream>
#include <windows.h>

#include "Source/model/model.h"
#include "Source/utilities/objutils.h"
#include "Source/graphics/Window.h"
#include "Source/graphics/shader.h"
#include "Source/graphics/buffer/indexbuffer.h"
#include "Source/graphics/buffer/vertexarray.h"
#include "Source/graphics/cameraRPG.h"
#include "Source/utilities/noiseutils.h"
#include "Source/utilities/Perlin.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <sysinfoapi.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define Window_TITLE "ZJU GAME DEVELOPEMENT"

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

Window window = Window(Window_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

using namespace std;

struct Node {
	GLuint v_num;
	GLuint n_num;
	Node(int v, int n) :v_num(v), n_num(n) {};
};
vector <Node> list;

int main() {

	//Model model = ObjUtils::readObj("Source/model/dragon.obj");
	vector <vec3> vertexes;
	vector <vec3> normals;


	NoiseUtils noise(0.5, 2);
	Perlin p = Perlin(200);


	vec3 test = vec3(0.0f, 0.0f, 1.0f);
	mat4 rotatex = mat4::rotation(90, AXISY);
	mat4 rotatey = mat4::rotation(90, AXISX);
	mat4 result = rotatex * rotatey;


	cout << test * rotatex << endl;
	cout << test * rotatey << endl;
	cout << test * rotatex * rotatey << endl;
	cout << test * rotatey * rotatex << endl;


	float k = 16;
	float gap = 0.05;
	vector <vec3> map;
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int z = 0; z < MAP_HEIGHT; z++){
			int x_p = x < 0 ? -x : x;
			int z_p = z < 0 ? -z : z;
			map.push_back(vec3(x - MAP_WIDTH /2, k * p.octavePerlin(x_p * gap, 0, z_p * gap,32, 1/32), z - MAP_HEIGHT /2));
		}
	}
	vector <float> ylist;
	vector<vec3> colors;
	vec3 lightgreen = vec3(0.8, 1.0, 0.0);
	vec3 deepgreen = vec3(0.6, 0.8, 0.2);
	vec3 brown = vec3(0.8, 0.6, 0.2);
	vec3 ice = vec3(0.8, 1.0, 1.0);

	int startPos = 0;
	for (int i = 0; i < MAP_HEIGHT - 1; i++) {//��
		int nowStartPos = startPos;
		int lineDownPos = i * MAP_WIDTH;
		int lineUpPos = (i + 1) * MAP_WIDTH;
		int nextPos[2] = { lineDownPos + 1,lineUpPos + 1 };
		for (int j = 0; j < 2 * (MAP_WIDTH - 1); j++) {
			int thirdPos = nextPos[nowStartPos];
			vertexes.push_back(map[lineDownPos]);
			vertexes.push_back(map[lineUpPos]);
			vertexes.push_back(map[thirdPos]);
			float avg_y = (map[lineDownPos].y + map[lineUpPos].y + map[thirdPos].y) / 3;
			if (avg_y < 7) {
				colors.push_back(brown);
				colors.push_back(brown);
				colors.push_back(brown);
			}
			else if (avg_y <= 10) {
				colors.push_back(lightgreen);
				colors.push_back(lightgreen);
				colors.push_back(lightgreen);
			}
			else if (avg_y <= 15) {
				colors.push_back(deepgreen);
				colors.push_back(deepgreen);
				colors.push_back(deepgreen);
			}
			else {
				colors.push_back(ice);
				colors.push_back(ice);
				colors.push_back(ice);
			}
			vec3 norml = vec3::cross(map[lineDownPos] - map[thirdPos], map[lineDownPos] - map[lineUpPos]);
			for (int i = 0; i < 3; i++) {
				normals.push_back(norml);
			}
			if (nowStartPos == 0) {
				lineDownPos++;
			}
			else {
				lineUpPos++;
				nextPos[0] = lineDownPos + 1;
				nextPos[1] = lineUpPos + 1;
			}
			nowStartPos = 1 - nowStartPos;
		}
	}



	//	vertexes.resize(model.m_vertexIndices.size() * 3);
	//	normals.resize(model.m_normalIndices.size() * 3);

		//vertexes.resize(36);
		//normals.resize(36);

		//for (int i = 0; i < model.m_vertexIndices.size(); i++) {
		//	for (int j = 0; j < 3; j++) {
		//		int v_index = model.m_vertexIndices[i].index[j];
		//		vertexes.push_back(model.m_vertexes[v_index]);
		//	}
		//}
		//


		//int count = 0;
		//for (int i = 0; i < model.m_vertexIndices.size(); i++) {
		//	for (int j = 0; j < 3; j++) {
		//		int n_index = model.m_normalIndices[i].index[j];
		//		normals.push_back(model.m_normals[n_index]);
		//	}
		//}



		//for (int i = 0; i < 36; i++) {
		//	scanf("%f %f %f %f %f %f", &vertexes[i].x, &vertexes[i].y, &vertexes[i].z,
		//		&normals[i].x, &normals[i].y, &normals[i].z);
		//}




	VertexArray vao;
	vao.addBuffer(new Buffer(&vertexes[0].x, vertexes.size() * 3, 3), 0);
	vao.addBuffer(new Buffer(&normals[0].x, normals.size() * 3, 3), 1);
	vao.addBuffer(new Buffer(&colors[0].x, colors.size() * 3, 3), 2);


	//	vao.addBuffer(new Buffer(&model.m_vertexes[0].x, model.m_vertexes.size() * 3, 3), 0);
	//	IndexBuffer ibo(&model.m_vertexIndices[0].index[0], model.m_vertexIndices.size() * 3);

	Shader shader("Source/shaders/basic.vert", "Source/shaders/basic.frag");
	shader.enable();

	GLfloat yaw = -90, pitch = 0;

	shader.setUniform3f("lightPos", vec3(0, 100 , 0));
	shader.setUniformMat4("perspective_matrix", mat4::perspective(45, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 200.0f));

	mat4 view = mat4();
	GLfloat x = -50, y = 0, z = -50;


	CameraRPG camera = CameraRPG(vec3(0.0f, 0.0f, -30.0f), vec3(0.0f, 30.0f, 30.0f));


	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;


	GLfloat preXPos = WINDOW_WIDTH / 2, preYPos = WINDOW_HEIGHT / 2;
	bool isFirstPress = true;

	float preTime = GetTickCount();
	float startTime = GetTickCount();
	vec4 colorday = vec4(1.0f, 1.0f, 1.0f,1.0f);
	vec4 colornight = vec4(0.2f, 0.2f, 0.6f, 1.0f);
	int isDay = 1;

	while (!window.closed()) {

		float nowTime = GetTickCount();
		if (nowTime - preTime > 10) {//����֡��

			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			camera.update(&window, preXPos, preYPos, deltaTime, isFirstPress);
//			shader.setUniformMat4("perspective_matrix", mat4::perspective(camera.getZoom(), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f));
			mat4 view = camera.getViewMatrix();
			shader.setUniformMat4("view_matrix", view);

			float nowTime = GetTickCount() - startTime;
			float a = nowTime / 30000;
			if (a == 1) {
				isDay = 1 - isDay;
				startTime = GetTickCount();
			}
			vec4 nowSkyColor;
			if (isDay) {
				nowSkyColor = vec4::LineInterpolation(colorday,colornight,a);
			}
			else{
				nowSkyColor = vec4::LineInterpolation(colornight,colorday, a);
			}
			shader.setUniform4f("NowlightColor", nowSkyColor);
			glClearColor(nowSkyColor.x, nowSkyColor.y, nowSkyColor.z, nowSkyColor.w);
			window.clear();

			vao.bind();
			//		ibo.bind();
			//		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
			glDrawArrays(GL_TRIANGLES, 0, vertexes.size());
			//		ibo.unbind();
			vao.unbind();

			window.update();
		}
	}
	shader.disable();

	return 0;
}


