#version 430 core

layout (location = 0) in vec4 in_Position;

out vec2 texCoords;

void main()
{
	gl_Position = in_Position;
	texCoords = 0.5 * (in_Position.xy + 1.0);
}