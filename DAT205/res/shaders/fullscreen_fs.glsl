#version 430 core

#define OUTPUT_R   1
#define OUTPUT_G   2
#define OUTPUT_B   3
#define OUTPUT_RG  4
#define OUTPUT_RB  5
#define OUTPUT_GB  6
#define OUTPUT_RGB 7

out vec3 out_Color;

uniform int u_TextureData = 7;

layout (binding = 0) uniform sampler2D u_Texture;

void main()
{
	out_Color = texelFetch(u_Texture, ivec2(gl_FragCoord.xy), 0).rgb;

	switch (u_TextureData)
	{
	case OUTPUT_R:
		out_Color = vec3(out_Color.r);
		break;

	case OUTPUT_G:
		out_Color = vec3(out_Color.g);
		break;

	case OUTPUT_B:
		out_Color = vec3(out_Color.b);
		break;

	case OUTPUT_RG:
		out_Color = vec3(out_Color.rg, 0.0);
		break;

	case OUTPUT_RB:
		out_Color = vec3(out_Color.r, 0.0, out_Color.b);
		break;

	case OUTPUT_GB:
		out_Color = vec3(0.0, out_Color.g, out_Color.b);
		break;

	case OUTPUT_RGB:
		break;
	}
}