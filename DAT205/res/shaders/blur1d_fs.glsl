#version 430 core

in vec2 texCoord;

out vec4 out_Color;

uniform vec2 u_ViewportSize;

uniform int u_VerticalBlur;

uniform int u_NumSamples;
uniform float u_Weights[64];

layout (binding = 0) uniform sampler2D u_Texture;

void main()
{
	vec2 coord = gl_FragCoord.xy;
	
	// Horizontal blur
	if (u_VerticalBlur == 0)
	{
		vec2 offset = vec2(1.0 / u_ViewportSize.x, 0.0);
		
		int minX = 1 - u_NumSamples;
		int maxX = u_NumSamples - 1;

		for (int x = minX; x <= maxX; x++)
			out_Color += u_Weights[abs(x)] * texture(u_Texture, texCoord + x * offset);
	}
	// Vertical blur
	else
	{
		vec2 offset = vec2(0.0, 1.0 / u_ViewportSize.y);
		
		int minY = 1 - u_NumSamples;
		int maxY = u_NumSamples - 1;

		for (int y = minY; y <= maxY; y++)
			out_Color += u_Weights[abs(y)] * texture(u_Texture, texCoord + y * offset);
	}
}