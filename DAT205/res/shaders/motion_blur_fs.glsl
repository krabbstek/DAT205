#version 430 core

in vec2 texCoord;

out vec4 out_Color;

uniform int u_NumSamples = 15;
uniform float u_VelocityScale = 1.0;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_ClipSpaceVelocityTexture;

void main()
{
	vec2 clipSpaceVelocity = u_VelocityScale * texture(u_ClipSpaceVelocityTexture, texCoord).rg / u_NumSamples;
	float invNumSamples = 1.0 / (max(u_NumSamples - 1, 1));
	float offset;
	float start = 0.5 * float(1 - u_NumSamples);
	for (int i = 0; i < u_NumSamples; i++)
	{
		offset = (float(i) - start) * invNumSamples;
		out_Color += texture(u_ColorTexture, texCoord - offset * clipSpaceVelocity);
	}

	out_Color /= u_NumSamples;
}