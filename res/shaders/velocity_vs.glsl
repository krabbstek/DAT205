#version 410 core

layout (location = 0) in vec4 in_Position;

out vec4 pos;
out vec4 prevPos;

uniform mat4 MVP;
uniform mat4 prevMVP;

void main()
{
	pos = MVP * in_Position;
	prevPos = prevMVP * in_Position;

	gl_Position = pos;
}