#version 410 core

in vec4 pos;
in vec4 prevPos;

out vec4 out_Color;

void main()
{
	out_Color = 10 * (pos - prevPos);
}