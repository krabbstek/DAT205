#version 430 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec2 in_TexCoord;

out vec3 worldSpacePosition_TCS;
out vec3 worldSpaceCenterPosition_TCS;
out vec2 texCoord_TCS;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ModelMatrix_normal;

void main()
{
	worldSpacePosition_TCS = (u_ModelMatrix * in_Position).xyz;
	worldSpaceCenterPosition_TCS = u_ModelMatrix[3].xyz;
	texCoord_TCS = in_TexCoord;
}