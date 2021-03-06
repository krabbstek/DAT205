#version 420

layout (location = 0) out vec3 out_Occlusion;

layout (binding = 10) uniform sampler2D u_ViewSpacePositionTexture;
layout (binding = 11) uniform sampler2D u_ViewSpaceNormalTexture;
layout (binding = 12) uniform sampler2D u_RandomAnglesTexture;

uniform vec3 u_Samples[64];
uniform int u_NumSamples;
uniform float u_Radius = 1.0;
uniform float u_Bias = 0.025;
uniform float u_RandomAngles[16 * 16];

uniform vec2 u_ViewportSize;
uniform float u_RandomAnglesTextureSize;

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
	// Read current pixel's view space position and normal
	ivec2 texCoord = ivec2(gl_FragCoord.xy);
	vec3 viewSpacePosition = texelFetch(u_ViewSpacePositionTexture, texCoord, 0).xyz;
	vec3 viewSpaceNormal = texelFetch(u_ViewSpaceNormalTexture, texCoord, 0).xyz;

	// Compute tangent and bitangent to build TBN matrix
	vec3 viewSpaceTangent = perpendicular(viewSpaceNormal);
	vec3 viewSpaceBitangent = normalize(cross(viewSpaceNormal, viewSpaceTangent));
	viewSpaceTangent = normalize(cross(viewSpaceBitangent, viewSpaceNormal));

	// Build TBN and rotate around normal by angle from randome angles texture
	texCoord &= (64 - 1);
	float randomAngle = texelFetch(u_RandomAnglesTexture, texCoord, 0).r;
	mat3 TBN = mat3(viewSpaceTangent, viewSpaceBitangent, viewSpaceNormal) * randomRotation(randomAngle);

	// Pre-allocation
	vec3 sampleVec;
	vec4 offset;
	float sampleDepth;
	float rangeCheck;
	float occlusion = 0.0;

	// Iterate through all sample vectors1
	for (int i = 0; i < u_NumSamples; i++)
	{
		// Transform sample vector toward normal
		sampleVec = TBN * u_Samples[i];
		sampleVec = viewSpacePosition + sampleVec * u_Radius;

		// Compute tex coord for sample position
		offset = u_ProjMatrix * vec4(sampleVec, 1.0);
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		// Check gbuffer depth at sample position
		sampleDepth = texture(u_ViewSpacePositionTexture, offset.xy).z;
		// Only take sample position and sample depth close enough into account
		rangeCheck = smoothstep(0.0, 1.0, u_Radius / abs(viewSpacePosition.z - sampleDepth));
		// Add to total occlusion
		occlusion += (sampleDepth >= sampleVec.z + u_Bias ? 1.0 : 0.0) * rangeCheck;
	}

	out_Occlusion = vec3(smoothstep(0.0, 1.0, 1.0 - (occlusion / u_NumSamples)));
}