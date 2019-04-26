#include "graphics/Light.h"

#include "ConstExpr.h"

Light g_GlobalLight = { vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1000.0f, 1000.0f, 1000.0f, 1.0f) };

float g_EnvironmentMultiplier = 1.0f;

float g_Time;
float g_DeltaTime = 0.0f;