#include "camera.h"


Camera::Camera(vec3 pos , vec3 up , GLfloat yaw , GLfloat pitch)
	: m_Front(vec3(0.0f, 0.0f, -1.0f)), m_Speed(SPEED), m_Sensitivity(SENSITIVTY), m_Zoom(ZOOM) {
	m_Pos = pos;
	m_WorldUp = up;
	m_Yaw = yaw;
	m_Pitch = pitch;
	updateCameraVector();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) 
	: m_Front(vec3(0.0f, 0.0f, -1.0f)), m_Speed(SPEED), m_Sensitivity(SENSITIVTY), m_Zoom(ZOOM) {
	m_Pos = vec3(posX, posY, posZ);
	m_WorldUp = vec3(upX, upY, upZ);
	m_Yaw = yaw;
	m_Pitch = pitch;
	updateCameraVector();
}

mat4 Camera::getViewMatrix() {
	return lookAt(m_Pos, m_Pos + m_Front, m_Up);
}

void Camera::updateCameraVector() {
	vec3 front;
	front.x = cos(mat4::toRadians(m_Yaw)) * cos(mat4::toRadians(m_Pitch));
	front.y = sin(mat4::toRadians(m_Pitch));
	front.z = sin(mat4::toRadians(m_Yaw)) * cos(mat4::toRadians(m_Pitch));
	m_Front = vec3::normalize(front);
	m_Right = vec3::normalize(vec3::cross(m_Front, m_WorldUp));//物体往左 感觉相机往右
	m_Up = vec3::normalize(vec3::cross(m_Right, m_Front));//物体往下 感觉相机往上
}

void Camera::processKeyboard(int action, GLfloat deltaTime) {
	GLfloat velocity = m_Speed * deltaTime;
	if (action == GLFW_KEY_W) {
		m_Pos += m_Front * velocity;
	}
	if (action == GLFW_KEY_S) {
		m_Pos -= m_Front * velocity;
	}
	if (action == GLFW_KEY_A) {
		m_Pos -= m_Right * velocity;
	}
	if (action == GLFW_KEY_D) {
		m_Pos += m_Right * velocity;
	}
	if (action == GLFW_KEY_Q) {
		m_Yaw -= velocity * m_Sensitivity;
	}
	if (action == GLFW_KEY_E) {
		m_Yaw += velocity * m_Sensitivity;
	}

	updateCameraVector();
}

void Camera::processMouseMovenment(GLfloat xoffset, GLfloat yoffset ,GLfloat deltaTime) {
	GLfloat velocity = m_Speed * deltaTime * m_Sensitivity;
	if (xoffset < -OMISSION) {
		m_Yaw += velocity;
	}
	if (xoffset > OMISSION) {
		m_Yaw -= velocity;
	}
	if (yoffset < -OMISSION) {
		m_Pitch -= velocity;
	}
	if (yoffset > OMISSION) {
		m_Pitch += velocity;
	}

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;
}

void Camera::processMouseScroll(GLfloat yoffest) {
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f) {
		m_Zoom -= yoffest;
	}
	if (m_Zoom < 1.0f) {
		m_Zoom = 1.0f;
	}
	if (m_Zoom > 45.0f) {
		m_Zoom = 45.0f;
	}
}

void Camera::move(Window * window, GLfloat & prePosX, GLfloat & prePosY, GLfloat deltaTime, bool & isFirst) {
	//键盘
	if (window->isKeyPressed(GLFW_KEY_W)) {
		processKeyboard(GLFW_KEY_W, deltaTime);
	}
	if (window->isKeyPressed(GLFW_KEY_S)) {
		processKeyboard(GLFW_KEY_S, deltaTime);
	}
	if (window->isKeyPressed(GLFW_KEY_A)) {
		processKeyboard(GLFW_KEY_A, deltaTime);
	}
	if (window->isKeyPressed(GLFW_KEY_D)) {
		processKeyboard(GLFW_KEY_D, deltaTime);
	}

	//鼠标移动
	if (window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT) || window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
		double nowPosX, nowPosY;
		GLfloat gl_nowPosX, gl_nowPosY;
		window->getMousePosition(nowPosX, nowPosY);
		gl_nowPosX = (GLfloat)nowPosX;
		gl_nowPosY = (GLfloat)nowPosY;
		GLfloat xoffet = gl_nowPosX - prePosX;
		GLfloat yoffet = gl_nowPosY - prePosY;
		if (isFirst) {
			xoffet = 0;
			yoffet = 0;
			isFirst = false;
		}
		prePosX = gl_nowPosX;
		prePosY = gl_nowPosY;
		processMouseMovenment(xoffet, yoffet, deltaTime);
		
	}

	//鼠标滚轮
	double zoom;
	window->getAsepct(zoom);
	m_Zoom = (GLfloat)zoom;

	updateCameraVector();
}

GLfloat Camera::getZoom() {
	//std::cout << m_Zoom << std::endl;
	return m_Zoom;
}


mat4 Camera::lookAt(const vec3 & cameraPos, const vec3 & cameraTarget, const vec3 & cameraUp) {

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