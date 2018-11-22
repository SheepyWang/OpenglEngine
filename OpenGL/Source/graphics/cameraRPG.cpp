#include "cameraRPG.h"


void CameraRPG::init() {
	m_Move = vec3(0.0f, 0.0f, 0.0f);
	m_Front = vec3(0.0, 0.0f, -1.0f);//默认朝向z轴负方向
	m_Zoom = m_Front;
	m_WorldUp = vec3(0.0f, 1.0f, 0.0f);
	m_Speed = SPEED;
	m_Sensitivity = SENSITIVTY;

}

void CameraRPG::vectorUpdate() {
	m_Right = vec3::normalize(vec3::cross(m_Front, m_WorldUp));
	m_Up = vec3::normalize(vec3::cross(m_Right, m_Front));
}

CameraRPG::CameraRPG(vec3 Target, vec3 Offset) {
	init();
	m_Target = Target;
	m_Offset = Offset;
}

CameraRPG::CameraRPG(GLfloat TargetX, GLfloat TargetY, GLfloat TargetZ, GLfloat OffsetX, GLfloat OffsetY, GLfloat OffsetZ) {
	init();
	m_Target = vec3(TargetX, TargetY, TargetZ);
	m_Offset = vec3(OffsetX, OffsetY, OffsetZ);
}

void CameraRPG::processKeyboard(Window * window, GLfloat deltaTime) {
	GLfloat velocity = m_Speed * deltaTime;
	if (window->isKeyPressed(GLFW_KEY_W)) {
		m_Move += m_Front * velocity;
	}
	if(window->isKeyPressed(GLFW_KEY_S)) {
		m_Move -= m_Front * velocity;
	}
	if(window->isKeyPressed(GLFW_KEY_A)) {
		m_Move -= m_Right * velocity;
	}
	if(window->isKeyPressed(GLFW_KEY_D)) {
		m_Move -= m_Right * velocity;
	}
	if (window->isKeyPressed(GLFW_KEY_Q)) {
		mat4 rotate = mat4::rotation(velocity * m_Sensitivity, vec3(0.0f, 1.0f, 0.0f));
		m_Front = m_Front * rotate;
		m_Offset = m_Offset * rotate;
		m_Zoom = m_Zoom * rotate;
	//	m_yaw += velocity * m_Sensitivity;
	}
	if (window->isKeyPressed(GLFW_KEY_E)) {
		mat4 rotate = mat4::rotation(-velocity * m_Sensitivity, vec3(0.0f, 1.0f, 0.0f));
		m_Front = m_Front * rotate;
		m_Offset = m_Offset * rotate;
		m_Zoom = m_Zoom * rotate;
	//	m_yaw -= velocity * m_Sensitivity;
	}
	if (window->isKeyPressed(GLFW_KEY_Z)) {
		mat4 rotate = mat4::rotation(velocity * m_Sensitivity, vec3(1.0f, 0.0f, 0.0f));
		m_Offset = m_Offset * rotate;
		m_Zoom = m_Zoom * rotate;
	//	m_pitch += velocity * m_Sensitivity;
	}
	if (window->isKeyPressed(GLFW_KEY_C)) {
		mat4 rotate = mat4::rotation(-velocity * m_Sensitivity, vec3(1.0f, 0.0f, 0.0f));
		m_Offset = m_Offset * rotate;
		m_Zoom = m_Zoom * rotate;
	//	m_pitch -= velocity * m_Sensitivity;
	}
}

void CameraRPG::update(Window * window, GLfloat & prePosX, GLfloat & prePosY, GLfloat deltaTime, bool & isFirst) {
	processKeyboard(window, deltaTime);
	vectorUpdate();
}

mat4 CameraRPG::getViewMatrix() {
	return CameraRPG::lookAt(m_Target + m_Offset + m_Move, m_Target + m_Move, m_Up);
}


mat4 CameraRPG::lookAt(const vec3 & cameraPos, const vec3 & cameraTarget, const vec3 & cameraUp) {

	vec3 Direction = vec3::normalize(cameraPos - cameraTarget);
	vec3 Right = vec3::normalize(vec3::cross(cameraUp, Direction));
	vec3 Up = vec3::cross(Direction, Right);

	mat4 ans = mat4(1);

	ans.elements[4 * 0 + 0] = Right.x;
	ans.elements[4 * 1 + 0] = Right.y;
	ans.elements[4 * 2 + 0] = Right.z;

	ans.elements[4 * 0 + 1] = Up.x;
	ans.elements[4 * 1 + 1] = Up.y;
	ans.elements[4 * 2 + 1] = Up.z;

	ans.elements[4 * 0 + 2] = Direction.x;
	ans.elements[4 * 1 + 2] = Direction.y;
	ans.elements[4 * 2 + 2] = Direction.z;

	vec3 dot = Right * cameraPos;
	ans.elements[4 * 3 + 0] = -(dot.x + dot.y + dot.z);
	dot = Up * cameraPos;
	ans.elements[4 * 3 + 1] = -(dot.x + dot.y + dot.z);
	dot = Direction * cameraPos;
	ans.elements[4 * 3 + 2] = -(dot.x + dot.y + dot.z);

	return ans;
}


