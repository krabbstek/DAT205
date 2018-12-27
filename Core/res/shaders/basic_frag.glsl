#version 330 core

precision highp float;

in vec2 texCoords;

out vec4 out_Color;

uniform sampler2D tex;

void main()
{
	out_Color = texture(tex, texCoords);
}