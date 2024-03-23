#version 430 core

layout (vertices = 4) out;

in vec3 worldSpacePosition_TCS[];
in vec3 worldSpaceNormal_TCS[];
in vec2 texCoord_TCS[];

out vec3 worldSpacePosition_TES[];
out vec3 worldSpaceNormal_TES[];
out vec2 texCoord_TES[];

uniform vec3 u_CameraPosition;

float GetTessLevel(float distance0, float distance1)
{
	float avgDistance = (distance0 + distance1) * 0.5;

	if (avgDistance <= 2.0)
		return 10.0;
	else if (avgDistance <= 5.0)
		return 7.0;
	else
		return 3.0;
}

void main()
{
	worldSpacePosition_TES[gl_InvocationID] = worldSpacePosition_TCS[gl_InvocationID];
	worldSpaceNormal_TES[gl_InvocationID] = worldSpaceNormal_TCS[gl_InvocationID];
	texCoord_TES[gl_InvocationID] = texCoord_TCS[gl_InvocationID];

	float eyeToVertexDistance0 = distance(u_CameraPosition, worldSpacePosition_TES[0]);
	float eyeToVertexDistance1 = distance(u_CameraPosition, worldSpacePosition_TES[1]);
	float eyeToVertexDistance2 = distance(u_CameraPosition, worldSpacePosition_TES[2]);

	gl_TessLevelOuter[0] = GetTessLevel(eyeToVertexDistance1, eyeToVertexDistance2);
	gl_TessLevelOuter[1] = GetTessLevel(eyeToVertexDistance2, eyeToVertexDistance0);
	gl_TessLevelOuter[2] = GetTessLevel(eyeToVertexDistance0, eyeToVertexDistance1);
	gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}