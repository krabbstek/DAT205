#version 430 core

layout (vertices = 4) out;

in vec3 worldSpacePosition_TCS[];
in vec3 worldSpaceCenterPosition_TCS[];
in vec2 texCoord_TCS[];

out vec3 worldSpacePosition_TES[];
out vec3 worldSpaceCenterPosition_TES[];
out vec2 texCoord_TES[];

uniform vec3 u_CameraPosition;

float GetTessLevel(float centerPositionCameraDistance)
{
	if (centerPositionCameraDistance <= 4.0)
		return 10.0;
	else if (centerPositionCameraDistance <= 10.0)
		return 2.0;
	else
		return 4.0;
}

void main()
{
	worldSpacePosition_TES[gl_InvocationID] = worldSpacePosition_TCS[gl_InvocationID];
	texCoord_TES[gl_InvocationID] = texCoord_TCS[gl_InvocationID];
	worldSpaceCenterPosition_TES[gl_InvocationID] = worldSpaceCenterPosition_TCS[gl_InvocationID];

	float tessLevel = GetTessLevel(distance(u_CameraPosition, worldSpaceCenterPosition_TCS[gl_InvocationID]));

	if (gl_InvocationID == 0)
	{
		gl_TessLevelOuter[0] = tessLevel;
		gl_TessLevelOuter[1] = tessLevel;
		gl_TessLevelOuter[2] = tessLevel;
		gl_TessLevelOuter[3] = tessLevel;
		gl_TessLevelInner[0] = tessLevel;
		gl_TessLevelInner[1] = tessLevel;
	}
}