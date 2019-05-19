#version 430 core

#define PI 3.141592653589793

in vec3 viewSpacePosition_FS;
in vec3 viewSpaceNormal_FS;
in vec2 texCoord_FS;

out vec3 out_Color;

uniform float u_EnvironmentMultiplier;
uniform int u_NumTileCols;
uniform int u_TileSize;
uniform mat4 u_ViewInverse;

uniform struct Material
{
	vec4 albedo;
	float emission;
	float reflectivity;
	float shininess;
	float metalness;
	float fresnel;
	float transparency;
} u_Material;

layout (binding = 7) uniform sampler2D u_IrradianceMap;
layout (binding = 8) uniform sampler2D u_ReflectionMap;
layout (binding = 9) uniform sampler2D u_SSAOTexture;

struct Light
{
	vec4 viewSpacePosition;
	vec4 color;
};

struct TileLights
{
	int offset;
	int lightCount;
};


/// Lights, containing view space position and color
layout (std430, binding = 3) readonly buffer LightBuffer
{
	Light lights[];
};

/// Integer array containing pure indices to above lights
layout (std430, binding = 4) readonly buffer LightIndexBuffer
{
	int totalNumberOfIndices;
	int lightIndices[];
};

/// Integer array of tile indices in lightIndices
layout (std430, binding = 5) readonly buffer TileIndexBuffer
{
	TileLights tileLights[];
};


float F(float wi_wh)
{
	return u_Material.fresnel + (1.0 - u_Material.fresnel) * max(pow(1.0 - wi_wh, 5.0), 0.0);
}

float D(float n_wh)
{
	return (u_Material.shininess + 2.0) * (1.0 / (2.0 * PI)) * max(pow(n_wh, u_Material.shininess), 0.0);
}

float G(float n_wi, float n_wo, float n_wh, float wo_wh)
{
	if (wo_wh <= 0.0)
		return 0.0;
	float coeff = 2.0 * n_wh / wo_wh;
	return min(1.0, min(coeff * n_wo, coeff * n_wi));
}

float brdf(float F, float D, float G, float n_wo, float n_wi)
{
	if (n_wi * n_wo == 0.0)
		return 0.0;
	return F * D * G / (4.0 * n_wi * n_wo);
}

int TileIndex(int tileCol, int tileRow)
{
	return tileCol + tileRow * u_NumTileCols;
}


vec3 n;
vec3 wi;
vec3 wo;
vec3 wh;
vec3 Li;
vec3 n_wi_Li;

float d2;
float inv_d2;

float n_wi;
float n_wo;
float n_wh;
float wi_wh;
float wo_wh;

float _F;
float _D;
float _G;
float _brdf;


vec3 calculateDirectIlluminationTerm()
{
	if (n_wi <= 0)
		return vec3(0.0);

	vec3 diffuseTerm = (1.0 / PI) * n_wi_Li;
	vec3 dielectricTerm = _brdf * n_wi_Li + (1.0 - _F) * diffuseTerm;
	vec3 metalTerm = _brdf * n_wi_Li;
	vec3 microfacetTerm = u_Material.metalness * metalTerm + (1.0 - u_Material.metalness) * dielectricTerm;

	vec3 directIlluminationTerm = u_Material.reflectivity * microfacetTerm + (1.0 - u_Material.reflectivity) * diffuseTerm;

	return directIlluminationTerm;
}

vec3 calculateIndirectIlluminationTerm()
{
	// Calculate the world-space position from view-space position
	vec4 worldPos = u_ViewInverse * vec4(viewSpacePosition_FS, 1.0);

	// Calculate the world-space normal from view-space normal
	vec3 worldSpaceNormal = normalize(mat3(u_ViewInverse) * n);

	// Calculate the spherical coordinates of the direction
	float theta = acos(clamp(worldSpaceNormal.y, -1.0, 1.0));
	float phi = atan(worldSpaceNormal.z, worldSpaceNormal.x);
	if (phi < 0.0)
		phi = phi + 2.0 * PI;

	// Use these to lookup the color in the environment map
	vec2 lookup = vec2(phi / (2.0 * PI), theta / PI);

	vec3 irradiance = u_EnvironmentMultiplier * texture(u_IrradianceMap, lookup).rgb;
	vec3 diffuseTerm = u_Material.albedo.rgb * (1.0 / PI) * irradiance;

	float ssao = texelFetch(u_SSAOTexture, ivec2(gl_FragCoord.xy), 0).r;

	return ssao * diffuseTerm;
}

vec3 calculateGlossyReflection()
{
	vec3 wiGlossy = reflect(-wo, n);
	float roughness = sqrt(sqrt(2.0 / (u_Material.shininess + 2.0)));
	vec3 dir = normalize(mat3(u_ViewInverse) * wiGlossy);

	// Calculate the spherical coordinates of the direction
	float theta = acos(max(-1.0f, min(1.0f, dir.y)));
	float phi = atan(dir.z, dir.x);
	if (phi < 0.0f)
		phi = phi + 2.0f * PI;

	// Use these to lookup the color in the environment map
	vec2 lookup = vec2(phi / (2.0 * PI), theta / PI);

	vec3 Li = u_EnvironmentMultiplier * textureLod(u_ReflectionMap, lookup, roughness * 7.0).rgb;

	vec3 whGlossy = normalize(wiGlossy + wo);

	float f = F(dot(wiGlossy, whGlossy));

	vec3 diffuseTerm = u_Material.albedo.rgb * (1.0 / PI) * dot(n, wiGlossy) * Li;
	vec3 dielectricTerm = f * Li + (1.0 - f) * diffuseTerm;
	vec3 metalTerm = f * u_Material.albedo.rgb * Li;
	vec3 microfacetTerm = u_Material.metalness * metalTerm + (1.0 - u_Material.metalness) * dielectricTerm;

	return u_Material.reflectivity * microfacetTerm + (1.0 - u_Material.reflectivity) * diffuseTerm;
}


void main()
{
	n = normalize(viewSpaceNormal_FS);
	wo = -normalize(viewSpacePosition_FS);

	vec3 directIlluminationTerm = vec3(0.0);

	vec3 diffuseTermPreComp = u_Material.albedo.rgb * (1.0 / PI);

	ivec2 tileCoords = ivec2(gl_FragCoord.xy) / u_TileSize;
	int tileIndex = TileIndex(tileCoords.x, tileCoords.y);
	int index;
	int lightIndicesOffset = tileLights[tileIndex].offset;
	Light light;
	int numLights = tileLights[tileIndex].lightCount;
	float lightThreshold;
	
	for (int i = 0; i < numLights; i++)
	{
		index = lightIndices[lightIndicesOffset + i];
		light = lights[index];
		lightThreshold = light.color.a;

		wi = light.viewSpacePosition.xyz - viewSpacePosition_FS.xyz;
		d2 = dot(wi, wi);
		inv_d2 = max(1.0 / d2 - lightThreshold, 0.0);
		wi = normalize(wi);
		n_wi = dot(n, wi);
		if (n_wi <= 0.0)
			continue;

		Li = light.color.rgb * inv_d2;
		wh = normalize(wi + wo);

		n_wo = max(dot(n, wo), 0.0);
		n_wh = max(dot(n, wh), 0.0);
		wi_wh = max(dot(wi, wh), 0.0);
		wo_wh = max(dot(wo, wh), 0.0);

		_F = F(wi_wh);
		_D = D(n_wh);
		_G = G(n_wi, n_wo, n_wh, wo_wh);

		_brdf = brdf(_F, _D, _G, n_wo, n_wi);

		n_wi_Li = n_wi * Li;

		directIlluminationTerm += calculateDirectIlluminationTerm();
	}

	vec3 indirectIlluminationTerm = calculateIndirectIlluminationTerm();
	vec3 glossyReflectionTerm = calculateGlossyReflection();
	vec3 emissionTerm = u_Material.emission * u_Material.albedo.rgb;

	out_Color = directIlluminationTerm
			  + indirectIlluminationTerm
			  + glossyReflectionTerm
			  + emissionTerm;
}