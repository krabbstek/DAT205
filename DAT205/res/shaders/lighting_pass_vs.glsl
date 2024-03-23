#version 430 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoords;

out vec3 viewSpacePosition;
out vec3 viewSpaceNormal;
out vec2 texCoords;

uniform mat4 u_MV = mat4(1.0);
uniform mat4 u_MVP = mat4(1.0);
uniform mat4 u_MV_normal = mat4(1.0);

void main()
{
	gl_Position = u_MVP * in_Position;

	viewSpacePosition = (u_MV * in_Position).xyz;
	viewSpaceNormal = mat3(u_MV_normal) * in_Normal;
	texCoords = in_TexCoords;
}