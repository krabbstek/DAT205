#version 330 core

layout(location = 0) in vec4 in_Position;

uniform mat4 transformation = mat4(1.0);

void main()
{
	gl_Position = transformation * in_Position;
}