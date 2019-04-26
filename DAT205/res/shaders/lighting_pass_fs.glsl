#version 430 core

#define PI 3.141592653589793

in vec4 viewSpacePosition;
in vec3 viewSpaceNormal;
in vec2 texCoords;

out vec3 out_Color;

layout (binding = 7) uniform sampler2D u_IrradianceMap;
layout (binding = 8) uniform sampler2D u_ReflectionMap;

uniform float u_EnvironmentMultiplier;

uniform mat4 u_ViewInverse = mat4(1.0);

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

uniform struct Light
{
	vec4 viewSpacePosition;
	vec4 color;
} u_Light;


float F(float wi_wh)
{
	return u_Material.fresnel + (1.0 - u_Material.fresnel) * pow(1.0 - wi_wh, 5.0);
}

float D(float n_wh)
{
	return (u_Material.shininess + 2.0) * (1.0 / (2.0 * PI)) * pow(n_wh, u_Material.shininess);
}

float G(float n_wi, float n_wo, float n_wh, float wo_wh)
{
	float coeff = 2.0 * n_wh / wo_wh;
	return min(1.0, min(coeff * n_wo, coeff * n_wi));
}

float brdf(float F, float D, float G, float n_wo, float n_wi)
{
	return F * D * G / (4.0 * n_wi * n_wo);
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

	vec3 diffuseTerm = u_Material.albedo.rgb * (1.0 / PI) * n_wi_Li;
	vec3 dielectricTerm = _brdf * n_wi_Li + (1.0 - _F) * diffuseTerm;
	vec3 metalTerm = _brdf * n_wi_Li;
	vec3 microfacetTerm = u_Material.metalness * metalTerm + (1.0 - u_Material.metalness) * dielectricTerm;

	vec3 directIlluminationTerm = u_Material.reflectivity * microfacetTerm + (1.0 - u_Material.reflectivity) * diffuseTerm;

	return directIlluminationTerm;
}

vec3 calculateIndirectIlluminationTerm()
{
	// Calculate the world-space position from view-space position
	vec4 worldPos = u_ViewInverse * viewSpacePosition;

	// Calculate the world-space normal from view-space normal
	vec3 worldSpaceNormal = normalize(mat3(u_ViewInverse) * n);

	// Calculate the spherical coordinates of the direction
	float theta = acos(max(-1.0f, min(1.0f, worldSpaceNormal.y)));
	float phi = atan(worldSpaceNormal.z, worldSpaceNormal.x);
	if (phi < 0.0f)
		phi = phi + 2.0f * PI;

	// Use these to lookup the color in the environment map
	vec2 lookup = vec2(phi / (2.0 * PI), theta / PI);

	vec3 irradiance = u_EnvironmentMultiplier * texture(u_IrradianceMap, lookup).rgb;
	vec3 diffuseTerm = u_Material.albedo.rgb * (1.0 / PI) * irradiance;

	return diffuseTerm;
}

vec3 calculateGlossyReflection()
{
	vec3 wi = reflect(-wo, n);
	float roughness = sqrt(sqrt(2.0 / (u_Material.shininess + 2.0)));
	vec3 dir = normalize(mat3(u_ViewInverse) * wi);

	// Calculate the spherical coordinates of the direction
	float theta = acos(max(-1.0f, min(1.0f, dir.y)));
	float phi = atan(dir.z, dir.x);
	if (phi < 0.0f)
		phi = phi + 2.0f * PI;

	// Use these to lookup the color in the environment map
	vec2 lookup = vec2(phi / (2.0 * PI), theta / PI);

	vec3 Li = u_EnvironmentMultiplier * textureLod(u_ReflectionMap, lookup, roughness * 7.0).rgb;

	vec3 diffuseTerm = u_Material.albedo.rgb * (1.0 / PI) * n_wi * Li;
	vec3 dielectricTerm = _F * Li + (1.0 - _F) * diffuseTerm;
	vec3 metalTerm = _F * u_Material.albedo.rgb * Li;
	vec3 microfacetTerm = u_Material.metalness * metalTerm + (1.0 - u_Material.metalness) * dielectricTerm;

	return u_Material.reflectivity * microfacetTerm + (1.0 - u_Material.reflectivity) * diffuseTerm;
}


void main()
{
	n = normalize(viewSpaceNormal);
	wo = -normalize(viewSpacePosition.xyz);

	wi = u_Light.viewSpacePosition.xyz - viewSpacePosition.xyz;
	d2 = dot(wi, wi);
	inv_d2 = 1.0 / d2;
	wi = normalize(wi);
	wh = normalize(wi + wo);

	Li = u_Light.color.rgb * inv_d2;

	n_wi = dot(n, wi);
	n_wo = dot(n, wo);
	n_wh = dot(n, wh);
	wi_wh = dot(wi, wh);
	wo_wh = dot(wo, wh);

	_F = F(wi_wh);
	_D = D(n_wh);
	_G = G(n_wi, n_wo, n_wh, wo_wh);

	_brdf = brdf(_F, _D, _G, n_wo, n_wi);

	n_wi_Li = n_wi * Li;

	vec3 directIlluminationTerm = calculateDirectIlluminationTerm();
	vec3 indirectIlluminationTerm = calculateIndirectIlluminationTerm();
	vec3 glossyReflectionTerm = calculateGlossyReflection();
	vec3 emissionTerm = u_Material.emission * u_Material.albedo.rgb;

	out_Color = directIlluminationTerm + indirectIlluminationTerm + glossyReflectionTerm + emissionTerm;
}