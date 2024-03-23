#version 420 core

precision highp float;

layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec2 in_TexCoords;

out vec2 texCoords;

uniform mat4 transformation = mat4(1.0);

void main()
{
	gl_Position = transformation * in_Position;
	texCoords = in_TexCoords;
}