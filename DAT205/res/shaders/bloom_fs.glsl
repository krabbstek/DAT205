#version 430 core

in vec2 texCoords;

out vec4 out_Color;

layout (binding = 0) uniform sampler2D u_ColorTexture;
layout (binding = 1) uniform sampler2D u_BloomTexture1;
layout (binding = 2) uniform sampler2D u_BloomTexture2;
layout (binding = 3) uniform sampler2D u_BloomTexture3;

uniform float u_BloomAlpha;

void main()
{
	out_Color = texture(u_ColorTexture, texCoords);
	out_Color += u_BloomAlpha * texture(u_BloomTexture1, texCoords);
	out_Color += u_BloomAlpha * texture(u_BloomTexture2, texCoords);
	out_Color += u_BloomAlpha * texture(u_BloomTexture3, texCoords);
}