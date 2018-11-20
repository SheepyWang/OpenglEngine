#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../maths/maths_func.h"
#include "Window.h"

#include <iostream>

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 10.0f;
const GLfloat SENSITIVTY = 5.0f;
const GLfloat ZOOM = 45.0f;
const GLfloat OMISSION = 2.0f;

class Camera {
private:
	vec3 m_Pos;
	vec3 m_Front;
	vec3 m_Right;
	vec3 m_Up;
	vec3 m_WorldUp;
	GLfloat m_Speed;
	GLfloat m_Sensitivity;
	GLfloat m_Zoom;
	GLfloat m_Yaw;
	GLfloat m_Pitch;

	void updateCameraVector();

public:
	Camera(vec3 pos = vec3(), vec3 up = vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw = YAW, GLfloat pitch = PITCH);
	mat4 getViewMatrix();
	void processKeyboard(int action ,GLfloat deltaTime);
	void processMouseMovenment(GLfloat xoffset, GLfloat yoffset,GLfloat deltaTime);
	void processMouseScroll(GLfloat yoffest);
	void move(Window * window, GLfloat & prePosX, GLfloat & prePosY, GLfloat deltaTime, bool & isFirst);
	GLfloat getZoom();
	static mat4 lookAt(const vec3 & cameraPos, const vec3 & cameraTarget, const vec3 & cameraUp);
};

#endif