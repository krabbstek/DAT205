#version 430 core

out vec3 out_Color;

uniform mat4 u_ProjectionMatrix;

layout (binding = 0) uniform sampler2D u_ViewSpacePositionTexture;

void main()
{
	vec4 position = texelFetch(u_ViewSpacePositionTexture, ivec2(gl_FragCoord.xy), 0);
	position = u_ProjectionMatrix * position;
	out_Color = vec3(position.z / position.w);
}