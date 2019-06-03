#pragma once

#include "ConstExpr.h"

#include "graphics/Light.h"
#include "graphics/RenderTechnique.h"

extern bool g_DisplayLightTilesOverlay;

extern Light g_GlobalLight;

extern float g_SSAOBias;
extern float g_SSAORadius;
extern int g_SSAOBlurSampleSize;
extern float g_SSAOBilateralBlurSigma;
extern float g_SSAODepthSigma;

extern float g_LightFalloffThreshold;

extern float g_EnvironmentMultiplier;

extern float g_GlowingParticleLightIntensityMultiplier;

extern float g_MotionBlurVelocityScale;

extern float g_BloomAlpha;
extern float g_BloomThreshold;
extern bool g_UseBloomLevelTexture[3];

extern bool g_UseNormalMap;

extern float g_Time;