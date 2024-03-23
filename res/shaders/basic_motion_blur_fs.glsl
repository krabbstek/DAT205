#version 420 core

in vec4 pos;
in vec4 prevPos;
in vec2 texCoords;

out vec4 out_Color;
out vec4 out_Velocity;

uniform float uVelocityScale = 10.0;
layout (binding = 0) uniform sampler2D tex;

void main()
{
	out_Color = texture(tex, texCoords);
	out_Velocity = uVelocityScale * (pos - prevPos);
}