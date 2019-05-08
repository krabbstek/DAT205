#version 430 core

in vec2 currentPosition;
in vec2 clipSpaceVelocity;
in vec3 emission;
in float lifetime;
in float lifelength;
in float radius;

out vec4 out_Color;

void main()
{
	float a = 0.4;

	vec2 position = gl_PointCoord - 0.5;
	float p2 = dot(position, position);
	if (p2 > 0.125)
		a = max(0.0, 3.2 * (0.25 - p2));

	out_Color = 4.0 * vec4(emission, a);
}