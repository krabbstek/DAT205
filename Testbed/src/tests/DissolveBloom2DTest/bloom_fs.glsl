#version 420 core

in vec2 texCoords;

out vec4 out_Color;

layout (binding = 0) uniform sampler2D u_FullscreenTexture;

uniform float u_TexCoordOffset = 0.0;
uniform int u_Vertical = 0;

uniform float u_KernelCoeff[16];
uniform int u_KernelSize;

void main()
{
	vec3 color = vec3(0.0, 0.0, 0.0);
	vec2 offset;
	vec4 tmp;
	float totalAlpha = 0.0f;

	if (u_Vertical == 1)
	{
		offset = vec2(0.0, u_TexCoordOffset);
	}
	else
	{
		offset = vec2(u_TexCoordOffset, 0.0);
	}

	vec2 coord1 = texCoords;
	vec2 coord2 = texCoords;

	tmp = texture(u_FullscreenTexture, texCoords);
	color += u_KernelCoeff[0] * tmp.rgb;
	totalAlpha += u_KernelCoeff[0] * tmp.a;
	
	for (int i = 1; i <= u_KernelSize; i++)
	{
		coord1 -= offset;
		coord2 += offset;
		tmp = texture(u_FullscreenTexture, coord1);
		color += u_KernelCoeff[i] * tmp.rgb;
		totalAlpha += u_KernelCoeff[i] * tmp.a;
		tmp = texture(u_FullscreenTexture, coord2);
		color += u_KernelCoeff[i] * tmp.rgb;
		totalAlpha += u_KernelCoeff[i] * tmp.a;
	}

	out_Color.rgb = color;
	out_Color.a = totalAlpha;
}