#pragma once

#include "math/math.h"

struct Camera
{
public:
	vec3 position;
	vec3 prevPosition;
	// Y-X-Z
	vec3 rotation;
	vec3 prevRotation;
	mat4 projectionMatrix;
	
	inline void Update()
	{
		prevPosition = position;
		prevRotation = rotation;
	}

	mat4 GetViewMatrix() const;
	mat4 GetPreviousViewMatrix() const;
	vec3 GetWorldSpaceForward() const;
};