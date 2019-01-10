#version 420 core

layout (location = 0) in vec4 in_Position;
layout (location = 1) in vec2 in_TexCoords;

out vec4 position;
out vec2 texCoords;

void main()
{
	position = in_Position;
	gl_Position = in_Position;
	texCoords = in_TexCoords;
}