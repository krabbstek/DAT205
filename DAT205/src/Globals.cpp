#include "graphics/Light.h"

#include "ConstExpr.h"

bool g_DisplayLightTilesOverlay = false;

Light g_GlobalLight = { vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1000.0f, 1000.0f, 1000.0f, 1.0f) };

float g_SSAOBias = 0.025f;
float g_SSAORadius = 1.0f;
int g_SSAOBlurSampleSize = 10;
float g_SSAOBilateralBlurSigma = 30.0f;
float g_SSAODepthSigma = 0.1f;

float g_LightFalloffThreshold = 0.01f;

float g_EnvironmentMultiplier = 1.0f;

float g_GlowingParticleLightIntensityMultiplier = 10.0f;

float g_MotionBlurVelocityScale = 1.0f;

float g_BloomAlpha = 0.25f;
float g_BloomThreshold = 1.0f;

bool g_UseNormalMap = true;

float g_Time;