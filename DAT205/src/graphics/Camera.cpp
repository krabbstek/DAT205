#include "Camera.h"

mat4 Camera::GetViewMatrix() const
{
	return mat4::RotateZ(-rotation.z) * mat4::RotateX(-rotation.x) * mat4::RotateY(-rotation.y) * mat4::Translate(-position);
}

mat4 Camera::GetInverseViewMatrix() const
{
	return mat4::Translate(position) * mat4::RotateY(rotation.y) * mat4::RotateX(rotation.x) * mat4::RotateZ(rotation.z);
}

mat4 Camera::GetPreviousViewMatrix() const
{
	return mat4::RotateZ(-prevRotation.z) * mat4::RotateX(-prevRotation.x) * mat4::RotateY(-prevRotation.y) * mat4::Translate(-prevPosition);
}

vec3 Camera::GetWorldSpaceForward() const
{
	vec4 forward = vec4(0.0f, 0.0f, -1.0f, 0.0f);
	forward = mat4::RotateY(rotation.y) * mat4::RotateX(rotation.x) * mat4::RotateZ(rotation.z) * forward;
	return forward.xyz;
}