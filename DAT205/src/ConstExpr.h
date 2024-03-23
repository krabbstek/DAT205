#pragma once

#include "math/math_util.h"

constexpr unsigned int g_WindowWidth = 1280;
constexpr unsigned int g_WindowHeight = 720;

constexpr float g_FOV = DegToRad(75.0f);
constexpr float g_AspectRatio = float(g_WindowWidth) / float(g_WindowHeight);
constexpr float g_NearPlaneDepth = 1.0f;
constexpr float g_FarPlaneDepth = 100.0f;

constexpr int g_NumGraphSamples = 240;
constexpr float g_LargeGraphSizeX = 320.0f;
constexpr float g_LargeGraphSizeY = 80.0f;
constexpr float g_SmallGraphSizeX = 320.0f;
constexpr float g_SmallGraphSizeY = 40.0f;

constexpr unsigned int BIT(unsigned int bit) { return 1 << bit; }