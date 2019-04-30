#version 430 core

precision highp float;

in vec2 texCoord;

layout (location = 0) out vec4 out_Color;
layout (location = 1) out vec3 out_BloomColor;

layout (binding = 6) uniform sampler2D u_EnvironmentMap; 

uniform mat4 u_InvVP;
uniform vec3 u_CameraPos;
uniform float u_EnvironmentMultiplier;

#define PI 3.141592653589793

void main() 
{
	// Calculate the world-space position of this fragment on the near plane
	vec4 pixelWorldPos = u_InvVP * vec4(texCoord * 2.0 - 1.0, 1.0, 1.0);
	pixelWorldPos = (1.0 / pixelWorldPos.w) * pixelWorldPos;

	// Calculate the world-space direction from the camera to that position
	vec3 dir = normalize(pixelWorldPos.xyz - u_CameraPos);

	// Calculate the spherical coordinates of the direction
	float theta = acos(max(-1.0, min(1.0, dir.y)));
	float phi = atan(dir.z, dir.x);
	if (phi < 0.0)
		phi = phi + 2.0f * PI;

	// Use these to lookup the color in the environment map
	vec2 lookup = vec2(phi / (2.0 * PI), theta / PI);
	out_Color = u_EnvironmentMultiplier * texture(u_EnvironmentMap, lookup);

	float brightness = out_Color.r + out_Color.g + out_Color.b;
	out_BloomColor = brightness >= 1.0 ? out_Color.rgb : vec3(0.0);
}