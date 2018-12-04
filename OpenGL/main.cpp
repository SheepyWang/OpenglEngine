#include <iostream>
#include <windows.h>
#include <time.h>

#include "Source/model/model.h"
#include "Source/utilities/objutils.h"
#include "Source/graphics/Window.h"
#include "Source/graphics/shader.h"
#include "Source/graphics/buffer/indexbuffer.h"
#include "Source/graphics/buffer/vertexarray.h"
#include "Source/graphics/cameraRPG.h"
#include "Source/utilities/noiseutils.h"
#include "Source/utilities/Perlin.h"
#include "Source/graphics/terrain.h"

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

struct cloud{
	vec3 pos;
};
cloud c[20];


int main() {

	//云生成随机位置
	srand(unsigned(time(NULL)));
	for (int i = 0; i < 20; i++) {
		c[i].pos.x = (rand() % MAP_WIDTH) - MAP_WIDTH/2;
		c[i].pos.y = 30 + rand() % 5;
		c[i].pos.z = (rand() % MAP_HEIGHT) - MAP_HEIGHT/2;
	}

	Model model = ObjUtils::readObj("Source/model/cloud.obj");

	vector <vec3> vertexes;
	vector <vec3> normals;
	vector<vec3> colors;

	vector <vec3> vertexes2;
	vector <vec3> normals2;
	vector<vec3> colors2;


	float NowTime = GetTickCount();

	cout << GetTickCount() - NowTime << endl;

	//地形生成
	Terrain t;
	t.Init(32, 1 / 32);
	t.Generate();
	t.DataTrasnform(vertexes, normals, colors);

	cout << GetTickCount() - NowTime << endl;

	int terrainSize = vertexes.size();

	//模型加载
	for (int i = 0; i < model.m_vertexIndices.size(); i++) {
		for (int j = 0; j < 3; j++) {
			int v_index = model.m_vertexIndices[i].index[j];
			vertexes2.push_back(model.m_vertexes[v_index]);
		}
	}

	int dargonSize = vertexes.size() - terrainSize;

	int count = 0;
	for (int i = 0; i < model.m_vertexIndices.size(); i++) {
		for (int j = 0; j < 3; j++) {
			int n_index = model.m_normalIndices[i].index[j];
			normals2.push_back(model.m_normals[n_index]);
			colors2.push_back(vec3(0.8, 1.0, 1.0));
		}
	}



	VertexArray vao;
	vao.addBuffer(new Buffer(&vertexes[0].x, vertexes.size() * 3, 3), 0);
	vao.addBuffer(new Buffer(&normals[0].x, normals.size() * 3, 3), 1);
	vao.addBuffer(new Buffer(&colors[0].x, colors.size() * 3, 3), 2);


	VertexArray vao2;
	vao2.addBuffer(new Buffer(&vertexes2[0].x, vertexes2.size() * 3, 3), 0);
	vao2.addBuffer(new Buffer(&normals2[0].x, normals2.size() * 3, 3), 1);
	vao2.addBuffer(new Buffer(&colors2[0].x, colors2.size() * 3, 3), 2);

	//	vao.addBuffer(new Buffer(&model.m_vertexes[0].x, model.m_vertexes.size() * 3, 3), 0);
	//	IndexBuffer ibo(&model.m_vertexIndices[0].index[0], model.m_vertexIndices.size() * 3);

	Shader shader("Source/shaders/basic.vert", "Source/shaders/basic.frag");
	shader.enable();
	shader.setUniform3f("lightPos", vec3(50, 100 , 50));
	shader.setUniformMat4("perspective_matrix", mat4::perspective(45, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 200.0f));

	mat4 view = mat4();

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
		if (nowTime - preTime > 10) {//降低帧数

			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			camera.update(&window, preXPos, preYPos, deltaTime, isFirstPress);
//			shader.setUniformMat4("perspective_matrix", mat4::perspective(camera.getZoom(), (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f));
			mat4 view = camera.getViewMatrix();
			shader.setUniformMat4("view_matrix", view);

			//日夜变化
			float nowTime = GetTickCount() - startTime;
			float a = nowTime / 30000;
			printf("%f\n", a);
			if (a >= 0.99f) {
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


			shader.setUniform3f("viewPos", camera.getCameraPos());


			glClearColor(nowSkyColor.x, nowSkyColor.y, nowSkyColor.z, nowSkyColor.w);
			window.clear();
			vao.bind();
			//		ibo.bind();
			//		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
			shader.setUniformMat4("model_matrix", mat4(1));
			glDrawArrays(GL_TRIANGLES, 0, vertexes.size());
			//		ibo.unbind();
			vao.unbind();

			vao2.bind();
			for (int i = 0; i < 20; i++) {
				c[i].pos.x -= deltaTime * 3;
				if (c[i].pos.x < -MAP_WIDTH / 2) {
					c[i].pos.x = MAP_WIDTH / 2;
					c[i].pos.z = rand() % MAP_HEIGHT - MAP_HEIGHT / 2;
				}
				mat4 model = mat4::translation(c[i].pos);
				shader.setUniformMat4("model_matrix", model);
				glDrawArrays(GL_TRIANGLES, 0, vertexes2.size());
			}
			vao2.unbind();


			window.update();
		}
	}
	shader.disable();

	return 0;
}


