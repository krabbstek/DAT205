#version 430 core

in vec2 texCoords;

out vec4 out_Color;

uniform float u_BloomThreshold = 1.0;
uniform int u_BloomResolutionLevel;

layout (binding = 0) uniform sampler2D u_InputColorTexture;

void main()
{
	out_Color = texelFetch(u_InputColorTexture, ivec2(gl_FragCoord.xy), u_BloomResolutionLevel);
	if (dot(out_Color.rgb, vec3(0.2126, 0.7152, 0.0722)) < u_BloomThreshold)
		out_Color = vec4(0.0);
}