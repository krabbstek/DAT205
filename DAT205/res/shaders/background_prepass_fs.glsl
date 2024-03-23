#version 430 core

precision highp float;

in vec2 texCoord;

layout (location = 0) out vec2 out_ClipSpaceVelocity;

uniform mat4 u_InvVP;
uniform mat4 u_PrevVP;

void main() 
{
	vec4 currentPos = vec4(2.0 * texCoord - 1.0, 1.0, 1.0);
	vec4 pixelWorldPos = u_InvVP * currentPos;
	vec4 prevPos = u_PrevVP * pixelWorldPos;
	prevPos.xy /= prevPos.w;

	out_ClipSpaceVelocity = 0.5 * (currentPos.xy - prevPos.xy);
}