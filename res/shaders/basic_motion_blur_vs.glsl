#version 420 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec2 in_TexCoords;

out vec4 pos;
out vec4 prevPos;
out vec2 texCoords;

uniform mat4 MVP;
uniform mat4 prevMVP;

void main()
{
	pos = MVP * in_Position;
	prevPos = prevMVP * in_Position;
	gl_Position = pos;
	texCoords = in_TexCoords;
}