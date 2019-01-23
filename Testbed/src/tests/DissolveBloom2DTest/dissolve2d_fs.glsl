#version 420 core

in vec2 texCoords;

out vec4 out_Color;
out vec4 out_BloomColor;

layout (binding = 0) uniform sampler2D u_Texture;
layout (binding = 1) uniform sampler2D u_DissolveTexture;

uniform float u_DissolveThreshold = 0;
uniform float u_EmissionThreshold = 0;
uniform vec4 u_EmissionColor = vec4(1.0, 1.0, 1.0, 1.0);
uniform float u_EmissionIntensity = 1.0;

void main()
{
	float dissolveTextureValue = texture(u_DissolveTexture, texCoords).r;

	if (dissolveTextureValue < u_DissolveThreshold)
		discard;

	if (dissolveTextureValue < u_EmissionThreshold)
	{
		out_Color = u_EmissionColor * u_EmissionIntensity;
		out_BloomColor = u_EmissionColor * u_EmissionIntensity;
	}
	else
	{
		out_Color = texture(u_Texture, texCoords);
		out_BloomColor = vec4(0.0);
	}
}