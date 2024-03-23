#version 430 core

out vec4 out_Color;


uniform int u_TileSize;
uniform int u_NumTileCols;
uniform float u_MaxNumLightsPerTile;

struct TileLights
{
	int offset;
	int lightCount;
};

/// Integer array of tile indices in lightIndices
layout (std430, binding = 5) readonly buffer TileIndexBuffer
{
	TileLights tileLights[];
};

int TileIndex(int tileCol, int tileRow)
{
	return tileCol + tileRow * u_NumTileCols;
}

float Fade(float low, float high, float intensity)
{
	float mid = 0.5 * (low + high);
	float range = 0.5 * (high - low);
	float x = 1.0 - clamp(abs(mid - intensity) / range, 0.0, 1.0);
	return smoothstep(0.0, 1.0, x);
}

vec3 HeatmapColor(int numLights)
{
	vec3 red   = vec3(1.0, 0.0, 0.0);
	vec3 green = vec3(0.0, 1.0, 0.0);
	vec3 blue  = vec3(0.0, 0.0, 1.0);

	float intensity = float(numLights) / float(u_MaxNumLightsPerTile);
	if (intensity > 0.4)
		return red;

	blue = Fade(-0.2, 0.2, intensity) * blue;
	green = Fade(0.0, 0.4, intensity) * green;
	red = Fade(0.2, 0.6, intensity) * red;
	return red + green + blue;
}

void main()
{
	ivec2 tile = ivec2(gl_FragCoord.xy) / u_TileSize;
	int numLights = tileLights[TileIndex(tile.x, tile.y)].lightCount;
	out_Color = vec4(HeatmapColor(numLights), 0.3);
}