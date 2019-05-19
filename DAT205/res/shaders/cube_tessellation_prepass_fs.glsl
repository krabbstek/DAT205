#version 430 core

#define PI 3.141592653589793

in vec3 viewSpacePosition_FS;
in vec3 viewSpaceNormal_FS;
in vec4 currentPosition;
in vec4 previousPosition;

out vec3 out_ViewSpacePosition;
out vec3 out_ViewSpaceNormal;
out vec2 out_ClipSpaceVelocity;

void main()
{
	out_ViewSpacePosition = viewSpacePosition_FS;
	out_ViewSpaceNormal = viewSpaceNormal_FS;

	vec2 currentPositionXY = currentPosition.xy / currentPosition.w;
	vec2 previousPositionXY = previousPosition.xy / previousPosition.w;
	out_ClipSpaceVelocity = 0.5 * (currentPositionXY - previousPositionXY);
}