#pragma once

#include "math/vec3.h"
#include "Camera.h"

class Renderer
{
public:
	Camera camera;
	vec3 clearColor;
};