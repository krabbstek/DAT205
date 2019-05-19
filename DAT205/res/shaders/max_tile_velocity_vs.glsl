#version 430 core

layout (location = 0) in vec4 in_Position;

out vec2 texCoord;

void main()
{
	gl_Position = in_Position;
	texCoord = 0.5 * (in_Position.xy + 1.0);
}