#pragma once

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../maths/maths_func.h"

class Camer {
public:
	static mat4 lookAt(const vec3 & cameraPos, const vec3 & cameraTarget, const vec3 & cameraUp);
};

#endif