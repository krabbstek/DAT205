#version 430 core

layout (quads, equal_spacing, ccw) in;

in vec3 worldSpacePosition_TES[];
in vec3 worldSpaceCenterPosition_TES[];
in vec2 texCoord_TES[];

out vec3 viewSpacePosition_FS;
out vec3 viewSpaceNormal_FS;
out vec4 currentPosition;
out vec4 previousPosition;

layout (binding = 4) uniform sampler2D u_DisplacementMap;

uniform float u_DisplacementFactor = 1.0;
uniform mat4 u_ViewMatrix;
uniform mat4 u_PreviousViewMatrix;
uniform mat4 u_ProjMatrix;


vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2, vec2 v3)
{
	vec2 p1 = mix(v0, v1, gl_TessCoord.x);
	vec2 p2 = mix(v3, v2, gl_TessCoord.x);
	return mix(p1, p2, gl_TessCoord.y);
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2, vec3 v3)
{
	vec3 p1 = mix(v0, v1, gl_TessCoord.x);
	vec3 p2 = mix(v3, v2, gl_TessCoord.x);
	return mix(p1, p2, gl_TessCoord.y);
}


void main()
{
	viewSpacePosition_FS = interpolate3D(worldSpacePosition_TES[0], worldSpacePosition_TES[1], worldSpacePosition_TES[2], worldSpacePosition_TES[3]);
	vec3 previousViewSpacePosition = viewSpacePosition_FS;
	viewSpacePosition_FS = (u_ViewMatrix * vec4(viewSpacePosition_FS, 1.0)).xyz;
	previousViewSpacePosition = (u_ViewMatrix * vec4(previousViewSpacePosition, 1.0)).xyz;
	
	vec3 viewSpaceCenterPosition_FS = (u_ViewMatrix * vec4(worldSpaceCenterPosition_TES[0], 1.0)).xyz;
	vec3 previousviewSpaceCenterPosition = (u_PreviousViewMatrix * vec4(worldSpaceCenterPosition_TES[0], 1.0)).xyz;
	viewSpaceNormal_FS = normalize(viewSpacePosition_FS - viewSpaceCenterPosition_FS);
	vec3 previousViewSpaceNormal = normalize(previousViewSpacePosition - previousviewSpaceCenterPosition);

	vec2 texCoord = interpolate2D(texCoord_TES[0], texCoord_TES[1], texCoord_TES[2], texCoord_TES[3]);

	float displacement = texture(u_DisplacementMap, texCoord).x;
	viewSpacePosition_FS += viewSpaceNormal_FS * displacement * u_DisplacementFactor;
	previousViewSpacePosition += previousViewSpaceNormal * displacement * u_DisplacementFactor;

	currentPosition = u_ProjMatrix * vec4(viewSpacePosition_FS, 1.0);
	previousPosition = u_ProjMatrix * vec4(previousViewSpacePosition, 1.0);

	gl_Position = u_ProjMatrix * vec4(viewSpacePosition_FS, 1.0);
}