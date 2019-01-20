#version 420 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec2 in_TexCoords;

out vec2 texCoords;

uniform mat4 u_MVP = mat4(1.0);

void main()
{
	gl_Position = u_MVP * in_Position;
	texCoords = in_TexCoords;
}