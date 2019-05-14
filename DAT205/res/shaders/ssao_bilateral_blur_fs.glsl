#version 430 core

in vec2 texCoord;

out float out_Color;

uniform int u_VerticalBlur;
uniform int u_SampleSize;
uniform float u_BlurSigma = 1.0;
uniform float u_DepthSigma = 1.0;
uniform vec2 u_InvViewportSize;

layout (binding = 10) uniform sampler2D u_ViewSpacePositionTexture;
layout (binding = 11) uniform sampler2D u_SSAOTexture;

void main()
{
	out_Color = texture(u_SSAOTexture, texCoord).r;

	const float texCoordFalloff = -1.0 / (u_BlurSigma * u_BlurSigma);
	const float depthFalloff = -1.0 / (u_DepthSigma * u_DepthSigma);
	float centerDepth = texture(u_ViewSpacePositionTexture, texCoord).z;
	float totalWeight = 1.0;
	float weight, sampleSSAO, sampleDepth, deltaDepth, c, fi;
	vec2 sampleCoord;

	if (u_VerticalBlur == 0)
	{
		const vec2 offset = vec2(u_InvViewportSize.x, 0.0);

		for (int i = 1; i <= u_SampleSize; i++)
		{
			fi = float(i);
			c = fi * fi * texCoordFalloff;
			const vec2 offsetCoord = fi * offset;

			// Right
			sampleCoord = texCoord + offsetCoord;
			sampleDepth = texture(u_ViewSpacePositionTexture, sampleCoord).z;
			sampleSSAO = texture(u_SSAOTexture, sampleCoord).r;
			deltaDepth = centerDepth - sampleDepth;
			weight = exp(c + deltaDepth * deltaDepth * depthFalloff);
			out_Color += sampleSSAO * weight;
			totalWeight += weight;

			// Left
			sampleCoord = texCoord - offsetCoord;
			sampleDepth = texture(u_ViewSpacePositionTexture, sampleCoord).z;
			sampleSSAO = texture(u_SSAOTexture, sampleCoord).r;
			deltaDepth = centerDepth - sampleDepth;
			weight = exp(c + deltaDepth * deltaDepth * depthFalloff);
			out_Color += sampleSSAO * weight;
			totalWeight += weight;
		}
	}
	else
	{
		const vec2 offset = vec2(0.0, u_InvViewportSize.y);

		for (int i = 1; i <= u_SampleSize; i++)
		{
			fi = float(i);
			c = fi * fi * texCoordFalloff;
			const vec2 offsetCoord = fi * offset;

			// Up
			sampleCoord = texCoord + offsetCoord;
			sampleDepth = texture(u_ViewSpacePositionTexture, sampleCoord).z;
			sampleSSAO = texture(u_SSAOTexture, sampleCoord).r;
			deltaDepth = centerDepth - sampleDepth;
			weight = exp(c + deltaDepth * deltaDepth * depthFalloff);
			out_Color += sampleSSAO * weight;
			totalWeight += weight;

			// Down
			sampleCoord = texCoord - offsetCoord;
			sampleDepth = texture(u_ViewSpacePositionTexture, sampleCoord).z;
			sampleSSAO = texture(u_SSAOTexture, sampleCoord).r;
			deltaDepth = centerDepth - sampleDepth;
			weight = exp(c + deltaDepth * deltaDepth * depthFalloff);
			out_Color += sampleSSAO * weight;
			totalWeight += weight;
		}
	}

	out_Color /= totalWeight;
}