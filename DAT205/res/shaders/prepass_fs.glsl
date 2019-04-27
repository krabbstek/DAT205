#version 430 core

in vec3 viewSpacePosition;
in vec3 viewSpaceNormal;
in vec4 currentPosition;
in vec4 previousPosition;

out vec3 out_ViewSpacePosition;
out vec3 out_ViewSpaceNormal;
out vec2 out_ClipSpaceVelocity;

uniform float u_VelocityScale = 1.0;

void main()
{
	out_ViewSpacePosition = viewSpacePosition;
	out_ViewSpaceNormal = normalize(viewSpaceNormal);

	vec2 currentPositionXY = currentPosition.xy / currentPosition.w;
	vec2 previousPositionXY = previousPosition.xy / previousPosition.w;

	out_ClipSpaceVelocity = u_VelocityScale * (currentPositionXY - previousPositionXY);
}