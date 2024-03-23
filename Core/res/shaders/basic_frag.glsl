#version 330 core

uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

out vec4 out_Color;

void main()
{
	out_Color = color;
}