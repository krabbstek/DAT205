#version 430 core

in vec2 clipSpaceVelocity;
in vec3 emission;
in float lifetime;
in float lifelength;

out vec4 out_Color;

void main()
{
	out_Color = vec4(0.2, 1.0, 0.2, 0.4);
}