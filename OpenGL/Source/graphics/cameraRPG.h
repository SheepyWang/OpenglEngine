#pragma once

#ifndef _CAMERA_RPG_H_
#define _CAMERA_RPG_H_

#include "../maths/maths_func.h"
#include "Window.h"

const GLfloat SPEED = 10.0f;
const GLfloat SENSITIVTY = 5.0f;
const GLfloat OMISSION = 2.0f;
const vec3 AXISX = vec3(1.0f, 0.0f, 0.0f);
const vec3 AXISY = vec3(0.0f, 1.0f, 0.0f);

class CameraRPG {
private:
	vec3 m_Move;	//����ƶ�����
	vec3 m_Target;	//�������
	vec3 m_Offset;	//�����Ŀ��֮��ƫ��
	vec3 m_Front;	//�������
	vec3 m_Zoom;	//����Ŵ���
	vec3 m_Up;		//���������
	vec3 m_Right;	//���������
	vec3 m_WorldUp;	//����������

	GLfloat m_Speed;
	GLfloat m_Sensitivity;
	GLfloat m_yaw;
	GLfloat m_pitch;

	void init();
	void vectorUpdate();
public:
	
	CameraRPG(vec3 Target, vec3 Offset);
	CameraRPG(GLfloat TargetX, GLfloat TargetY, GLfloat TargetZ, GLfloat OffsetX, GLfloat OffsetY, GLfloat OffsetZ);
	mat4 getViewMatrix();
	void processKeyboard(Window * window, GLfloat deltaTime);
	void processMouseMovenment(GLfloat xoffset, GLfloat yoffset, GLfloat deltaTime);
	void processMouseScroll(GLfloat yoffest);
	void update(Window * window, GLfloat & prePosX, GLfloat & prePosY, GLfloat deltaTime, bool & isFirst);
	static mat4 lookAt(const vec3 & cameraPos, const vec3 & cameraTarget, const vec3 & cameraUp);
};

#endif // !_CAMERA_RPG_H_
