#version 420

layout (location = 0) out vec3 out_Occlusion;

layout (binding = 10) uniform sampler2D u_ViewSpacePositionTexture;
layout (binding = 11) uniform sampler2D u_ViewSpaceNormalTexture;
layout (binding = 12) uniform sampler2D u_RandomAnglesTexture;

uniform vec3 u_Samples[64];
uniform int u_NumSamples;
uniform float u_KernelSize = 1.0;
uniform float u_Radius = 1.0;

uniform vec2 u_ViewportSize;

uniform mat4 u_ProjMatrix;
uniform mat4 u_InverseProjMatrix;

vec3 homogenize(vec4 v)
{
	return v.xyz * (1.0 / v.w);
}

vec3 perpendicular(vec3 v)
{
	vec3 av = abs(v);
	if (av.x < av.y)
		if (av.x < av.z)
			return normalize(vec3(0.0f, -v.z, v.y));
		else
			return normalize(vec3(-v.y, v.x, 0.0f));
	else
		if (av.y < av.z)
			return normalize(vec3(-v.z, 0.0f, v.x));
		else
			return normalize(vec3(-v.y, v.x, 0.0f));
}

mat3 randomRotation(float randomAngle)
{
	float c = cos(randomAngle);
	float s = sin(randomAngle);

	return mat3(vec3(c, s, 0.0), vec3(-s, c, 0.0), vec3(0.0, 0.0, 1.0));
}

void main()
{
	vec3 viewSpacePosition = texelFetch(u_ViewSpacePositionTexture, ivec2(gl_FragCoord.xy), 0).xyz;
	vec3 viewSpaceNormal = normalize(texelFetch(u_ViewSpaceNormalTexture, ivec2(gl_FragCoord.xy), 0).xyz);

	vec3 viewSpaceTangent = perpendicular(viewSpaceNormal);
	vec3 viewSpaceBitangent = normalize(cross(viewSpaceNormal, viewSpaceTangent));

	float randomAngle = texture(u_RandomAnglesTexture, gl_FragCoord.xy / textureSize(u_RandomAnglesTexture, 0)).r;
	mat3 tbn = mat3(viewSpaceTangent, viewSpaceBitangent, viewSpaceNormal) * randomRotation(randomAngle);

	int numVisibleSamples = 0;
	int numValidSamples = 0;

	vec4 viewSpaceSamplePosition;

	for (int i = 0; i < u_NumSamples; i++)
	{
		// Project hemishere sample onto the local base
		vec3 s = tbn * u_Samples[i];

		// compute view-space position of sample
		vec3 viewSpaceSamplePosition = viewSpacePosition + s * u_KernelSize;

		// compute the ndc-coords of the sample
		vec3 sampleCoordsNDC = homogenize(u_ProjMatrix * vec4(viewSpaceSamplePosition, 1.0));

		// Sample the depth-buffer at a texture coord based on the ndc-coord of the sample
		// Find the view-space coord of the blocker
		vec3 viewSpaceBlockerPosition = texture(u_ViewSpacePositionTexture, 0.5 * sampleCoordsNDC.xy + 0.5).xyz;

		// Check that the blocker is closer than kernel_size to vs_pos
		// (otherwise skip this sample)
		vec3 dp = viewSpaceBlockerPosition - viewSpacePosition.xyz;
		if (dot(dp, dp) > u_Radius * u_Radius)
			continue;

		// Check if the blocker pos is closer to the camera than our
		// fragment, otherwise, increase num_visible_samples
		if (dot(viewSpaceBlockerPosition, viewSpaceBlockerPosition) > dot(viewSpacePosition.xyz, viewSpacePosition.xyz))
			numVisibleSamples++;

		numValidSamples++;
	}

	float hemisphericalVisibility = 0.0;
	if (numValidSamples > 0)
		hemisphericalVisibility = float(numVisibleSamples) / float(numValidSamples);

	out_Occlusion = vec3(hemisphericalVisibility);
}