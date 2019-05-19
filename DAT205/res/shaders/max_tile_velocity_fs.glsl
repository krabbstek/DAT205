#version 430 core

in vec2 texCoord;

out vec2 out_MaxVelocity;

uniform int u_VerticalSampling;
uniform int u_TileSize;

layout (binding = 0) uniform sampler2D u_VelocityTexture;

void main()
{
	if (u_VerticalSampling == 0)
	{
		ivec2 baseCoord = ivec2(gl_FragCoord.xy);
		baseCoord.x *= u_TileSize;
		ivec2 coord = baseCoord;

		out_MaxVelocity = texelFetch(u_VelocityTexture, baseCoord, 0).xy;
		float maxVelocity = length(out_MaxVelocity);

		for (int x = 1; x < u_TileSize; x++)
		{
			coord.x = baseCoord.x + x;
			const vec2 velocity = texelFetch(u_VelocityTexture, coord, 0).xy;
			const float absVelocity = length(velocity);
			if (absVelocity > maxVelocity)
			{
				out_MaxVelocity = velocity;
				maxVelocity = absVelocity;
			}
		}
	}
	else
	{
		ivec2 baseCoord = ivec2(gl_FragCoord.xy);
		baseCoord.y *= u_TileSize;
		ivec2 coord = baseCoord;

		out_MaxVelocity = texelFetch(u_VelocityTexture, baseCoord, 0).xy;
		float maxVelocity = length(out_MaxVelocity);

		for (int y = 1; y < u_TileSize; y++)
		{
			coord.y = baseCoord.y + y;
			const vec2 velocity = texelFetch(u_VelocityTexture, coord, 0).xy;
			const float absVelocity = length(velocity);
			if (absVelocity > maxVelocity)
			{
				out_MaxVelocity = velocity;
				maxVelocity = absVelocity;
			}
		}
	}
}