#version 330 core

precision highp float;

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec2 in_TexCoords;

smooth out vec4 pos;
smooth out vec4 prevPos;
smooth out vec2 texCoords;

uniform mat4 MVP = mat4(1.0);
uniform mat4 prevMVP = mat4(1.0);

void main()
{
	pos = MVP * in_Position;
	prevPos = prevMVP * in_Position;
	gl_Position = pos;
	texCoords = in_TexCoords;
}