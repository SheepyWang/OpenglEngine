#include "mat4.h"
#include "vec3.h"

#define _USE_MATH_DEFINES
#include <math.h>

float toRadians(float degrees) {
	return degrees * M_PI / 180.0f;
}


mat4::mat4() {
	memset(elements, 0, sizeof(elements));
}

mat4::mat4(const float a) {
	memset(elements, 0, sizeof(elements));
	for (int i = 0; i < 16; i+=5) {
		elements[i] = a;
	}
}

mat4 & mat4::multiply(const mat4 & m4) {

	mat4 tmp;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int pos = 4 * i + j;
			float sum = 0;
			for (int z = 0; z < 4; z++) {
				sum += this->elements[i * 4 + z] * m4.elements[i + z * 4];
			}
			tmp.elements[pos] = sum;
		}
	}

	for (int i = 0; i < 16; i++) {
		this->elements[i] = tmp.elements[i];
	}

	return *this;
}


mat4 operator * (const  mat4 & left, mat4 right){
	return right.multiply(left);
}

mat4& mat4::operator*=(const mat4 & m4) {
	return multiply(m4);
}

mat4 mat4::identity() {
	return mat4(1.0f);
}

mat4 mat4::transposed(const mat4 & m) {
	mat4 tm;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int orgPos = i * 4 + j;
			int dstPos = j * 4 + i;
			tm.elements[dstPos] = m.elements[orgPos];
		}
	}
	return tm;
}

mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far) {
	mat4 om;
	om.elements[0] = 2 / (right - left);
	om.elements[5] = 2 / (top - bottom);
	om.elements[10] = -2 / (far - near);
	om.elements[15] = 1;
	om.elements[12] = -(right + left) / (right - left);
	om.elements[13] = -(top + bottom) / (top - bottom);
	om.elements[14] = -(far + near) / (far - near);
	return om;
}

mat4 mat4::perspective(float fov, float aspectRatio, float near, float far) {
	mat4 pm;
	float fovRad = toRadians(fov / 2);
	pm.elements[0] = 1 / (aspectRatio * tan(fovRad));
	pm.elements[5] = 1 / (tan(fovRad));
	pm.elements[11] = -1;
	pm.elements[10] = -(far + near) / (far- near);
	pm.elements[14] = -2 * far*near / (far - near);
	return pm;
}

mat4 mat4::translation(const vec3 &translation) {
	mat4 tram(1);
	tram.elements[12] = translation.x;
	tram.elements[13] = translation.y;
	tram.elements[14] = translation.z;
	return tram;
}

mat4 mat4::rotation(float angle, const vec3 &axis) {

	mat4 rotm;

	float angleRad = toRadians(angle);
	float c = cos(angleRad);
	float c1 = 1 - c;
	float s = sin(angleRad);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	memset(rotm.elements, 0, sizeof(elements));

	rotm.elements[0] = x * x * c1 + c;
	rotm.elements[1] = x * y * c1 + z * s;
	rotm.elements[2] = x * z * c1 - y * s;
	rotm.elements[4] = x * y * c1 - z * s;
	rotm.elements[5] = y * y * c1 + c;
	rotm.elements[6] = y * z * c1 + x * s;
	rotm.elements[8] = x * z * c1 + y * s;
	rotm.elements[9] = y * z * c1 - x * s;
	rotm.elements[10] = z * z * c1 + c;
	rotm.elements[15] = 1;

	return rotm;
}

mat4 mat4::scale(const vec3 &scale) {
	mat4 sm;
	memset(sm.elements, 0, sizeof(elements));
	sm.elements[0] = scale.x;
	sm.elements[5] = scale.y;
	sm.elements[10] = scale.z;
	sm.elements[15] = 1;
	return sm;
}