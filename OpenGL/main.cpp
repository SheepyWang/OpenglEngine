#include <iostream>

#include "Source/model/model.h"
#include "Source/utilities/objutils.h"
#include "Source/graphics/Window.h"
#include "Source/graphics/shader.h"
#include "Source/graphics/buffer/indexbuffer.h"
#include "Source/graphics/buffer/vertexarray.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define Window_TITLE "ZJU GAME DEVELOPEMENT"

using namespace std;

int main() {



	Model model = ObjUtils::readObj("Source/model/dragon.obj");

	//cout << "Vertex" << endl;
	//for (int i = 1; i <= model.m_vertexes.size(); i++) {
	//	cout << model.m_vertexes[i - 1];
	//}

	//cout << "Indices" << endl;
	//for (int i = 1; i <= model.m_vertexIndices.size(); i++) {
	//	cout << model.m_vertexIndices[i - 1];
	//}


	GLfloat vertexes[] = {
		0.5,0,0,
		-0.5,0,0,
		0,0.5,0,0,
//		0,-0.5,0
	};

	GLushort indices[] = {
		0,1,2,
		0,1,3
	};
	

	Window window = Window(Window_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

	VertexArray vao;
//	vao.addBuffer(new Buffer(vertexes, 3 * 3, 3), 0);
	vao.addBuffer(new Buffer(&model.m_vertexes[0].x, model.m_vertexes.size() * 3 ,3), 0);
	IndexBuffer ibo(&model.m_vertexIndices[0].index[0], model.m_vertexIndices.size() * 3);
//	IndexBuffer ibo(indices, 6);

	Shader shader("Source/shaders/basic.vert","Source/shaders/basic.frag");
	shader.enable();

	shader.setUniformMat4("scale_matrix", mat4::scale(vec3(0.1,0.1,0.1)));
//	shader.setUniformMat4("model_matrix", mat4::translation(vec3(0, 0, 0)));
	shader.setUniform4f("vertexColor", vec4(0.2f, 0.3f, 0.8f, 1.0f));
//	shader.setUniform2f("light_position", vec2(0.0, 0.0));

	while (!window.closed()){

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		window.clear();

		vao.bind();
		ibo.bind();
		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
		ibo.unbind();
//		glDrawArrays(GL_TRIANGLES, 0, 8);
		vao.unbind();

		window.update();
	}
	shader.disable();

	return 0;
}