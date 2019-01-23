#version 420 core

in vec2 texCoords;

out vec4 out_Color;

float exposure = 0.0;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_BloomTexture;

void main()
{
	vec3 color = 0.5 * texture(u_ColorTexture, texCoords).rgb;
	vec4 bloom = texture(u_BloomTexture, texCoords);

	color += bloom.rgb * bloom.a;
	out_Color.rgb = color;
	out_Color.a = 1.0;
}