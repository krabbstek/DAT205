#version 430 core

in vec2 texCoords;

out vec4 out_Color;

uniform float u_BloomThreshold = 1.0;
uniform int u_BloomResolutionLevel;

layout (binding = 0) uniform sampler2D u_InputColorTexture;

void main()
{
	out_Color = texelFetch(u_InputColorTexture, ivec2(gl_FragCoord.xy), u_BloomResolutionLevel);
	float brightness = dot(out_Color.rgb, vec3(0.2126, 0.7152, 0.0722)) / u_BloomThreshold;
	brightness = smoothstep(0.0, 1.0, (clamp(brightness, 0.8, 1.0) - 0.8) * 5.0);
	out_Color.rgb *= brightness;
}