#version 430 core

out float out_Variance;

uniform ivec2 u_NumTiles;

layout (binding = 0) uniform sampler2D u_MaxTileVelocityTexture;

void main()
{
	ivec2 coord = ivec2(gl_FragCoord.xy);
	float centerLength = length(texelFetch(u_MaxTileVelocityTexture, coord, 0).xy);

	ivec2 minCoord;
	minCoord.x = max(coord.x - 1, 0);
	minCoord.y = max(coord.y - 1, 0);

	ivec2 maxCoord;
	maxCoord.x = min(coord.x + 2, u_NumTiles.x);
	maxCoord.y = min(coord.y + 2, u_NumTiles.y);

	float variance;
	float currentLength;
	int numSamples = 0;
	for (coord.x = minCoord.x; coord.x < maxCoord.x; coord.x++)
	{
		for (coord.y = minCoord.y; coord.y < maxCoord.y; coord.y++)
		{
			currentLength = length(texelFetch(u_MaxTileVelocityTexture, coord, 0).xy);
			variance += currentLength * centerLength;
		}
	}

	out_Variance = 1.0f - variance / numSamples;
}