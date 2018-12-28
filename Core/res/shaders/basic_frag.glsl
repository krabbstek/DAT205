#version 420 core

precision highp float;

in vec2 texCoords;

out vec4 out_Color;

layout (binding = 0) uniform sampler2D tex;
layout (binding = 1) uniform sampler2D colorOverlay;

void main()
{
	out_Color = texture(tex, texCoords) * texture(colorOverlay, texCoords);
}