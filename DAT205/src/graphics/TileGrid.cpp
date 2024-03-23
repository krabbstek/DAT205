#include "TileGrid.h"

#include "Globals.h"

#include <algorithm>

TileGrid::TileGrid()
{
	// Make sure memory is reset
	memset(m_TileIndexMemoryLayout, 0, sizeof(m_TileIndexMemoryLayout));

	// Compute tile planes
	float tanY = tanf(0.5f * g_FOV);
	float tanX = g_AspectRatio * tanY;
	float halfTanPerTile = tanY * (1.0f / g_NumTileRows);

	vec3 normal;

	// Left planes
	for (int col = 0, numHalfTanPerTile = -g_NumTileCols; col < g_NumTileCols; col++, numHalfTanPerTile += 2)
	{
		float tan = halfTanPerTile * float(numHalfTanPerTile);
		normal.x = 1.0f;
		normal.y = 0.0f;
		normal.z = tan;
		normal.Normalize();

		leftPlanes[col].a = normal.x;
		leftPlanes[col].c = normal.z;
	}

	// Right planes
	for (int col = 0, numHalfTanPerTile = -(g_NumTileCols - 2); col < g_NumTileCols; col++, numHalfTanPerTile += 2)
	{
		float tan = halfTanPerTile * float(numHalfTanPerTile);
		normal.x = -1.0f;
		normal.y = 0.0f;
		normal.z = -tan;
		normal.Normalize();

		rightPlanes[col].a = normal.x;
		rightPlanes[col].c = normal.z;
	}

	// Bottom planes
	for (int row = 0, numHalfTanPerTile = -g_NumTileRows; row < g_NumTileRows; row++, numHalfTanPerTile += 2)
	{
		float tan = halfTanPerTile * float(numHalfTanPerTile);
		normal.x = 0.0f;
		normal.y = 1.0f;
		normal.z = tan;
		normal.Normalize();

		bottomPlanes[row].b = normal.y;
		bottomPlanes[row].c = normal.z;
	}

	// Top planes
	for (int row = 0, numHalfTanPerTile = -(g_NumTileRows - 2); row < g_NumTileCols; row++, numHalfTanPerTile += 2)
	{
		float tan = halfTanPerTile * float(numHalfTanPerTile);
		normal.x = 0.0f;
		normal.y = -1.0f;
		normal.z = -tan;
		normal.Normalize();

		topPlanes[row].b = normal.y;
		topPlanes[row].c = normal.z;
	}
}