#version 330 core

layout(location = 0) in vec4 in_Position;

void main()
{
	gl_Position = in_Position;
}