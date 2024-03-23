#version 430 core

in vec2 texCoord;

out float out_Color;

uniform int u_SampleSize;
uniform float u_BlurSigma = 1.0;
uniform float u_DepthSigma = 1.0;
uniform vec2 u_InvViewportSize;

layout (binding = 10) uniform sampler2D u_ViewSpacePositionTexture;
layout (binding = 11) uniform sampler2D u_SSAOTexture;

void main()
{
	out_Color = texture(u_SSAOTexture, texCoord).r;

	float totalWeight = 1.0;
	float centerDepth = texture(u_ViewSpacePositionTexture, texCoord).z;
	float texCoordFalloff, depthFalloff, weight, sampleSSAO, sampleDepth, deltaDepth, c;
	vec2 offsetCoord;

	texCoordFalloff = -1.0 / (u_BlurSigma * u_BlurSigma);
	depthFalloff = -1.0 / (u_DepthSigma * u_DepthSigma);
	vec2 offsetX = vec2(u_InvViewportSize.x, 0.0);
	vec2 offsetY = vec2(0.0, u_InvViewportSize.y);
	for (int i = 1; i <= u_SampleSize; i++)
	{
		c = float(i) * float(i) * texCoordFalloff;

		// Right
		offsetCoord = texCoord + float(i) * offsetX;
		sampleDepth = texture(u_ViewSpacePositionTexture, offsetCoord).z;
		sampleSSAO = texture(u_SSAOTexture, offsetCoord).r;
		deltaDepth = centerDepth - sampleDepth;
		weight = exp(c + deltaDepth * deltaDepth * depthFalloff);
		out_Color += sampleSSAO * weight;
		totalWeight += weight;
		
		// Left
		offsetCoord = texCoord - float(i) * offsetX;
		sampleDepth = texture(u_ViewSpacePositionTexture, offsetCoord).z;
		sampleSSAO = texture(u_SSAOTexture, offsetCoord).r;
		deltaDepth = centerDepth - sampleDepth;
		weight = exp(c + deltaDepth * deltaDepth * depthFalloff);
		out_Color += sampleSSAO * weight;
		totalWeight += weight;

		// Up
		offsetCoord = texCoord + float(i) * offsetY;
		sampleDepth = texture(u_ViewSpacePositionTexture, offsetCoord).z;
		sampleSSAO = texture(u_SSAOTexture, offsetCoord).r;
		deltaDepth = centerDepth - sampleDepth;
		weight = exp(c + deltaDepth * deltaDepth * depthFalloff);
		out_Color += sampleSSAO * weight;
		totalWeight += weight;
		
		// Down
		offsetCoord = texCoord - float(i) * offsetY;
		sampleDepth = texture(u_ViewSpacePositionTexture, offsetCoord).z;
		sampleSSAO = texture(u_SSAOTexture, offsetCoord).r;
		deltaDepth = centerDepth - sampleDepth;
		weight = exp(c + deltaDepth * deltaDepth * depthFalloff);
		out_Color += sampleSSAO * weight;
		totalWeight += weight;
	}

	out_Color /= totalWeight;
}