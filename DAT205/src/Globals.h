#pragma once

#include "ConstExpr.h"

#include "graphics/Light.h"
#include "graphics/RenderTechnique.h"

extern Light g_GlobalLight;

extern float g_SSAORadius;
extern float g_SSAOKernelSize;
extern int g_SSAOBlurSampleSize;
extern float g_SSAOBilateralBlurSigma;
extern float g_SSAODepthSigma;

extern float g_EnvironmentMultiplier;

extern float g_MotionBlurVelocityScale;

extern float g_BloomAlpha;
extern float g_BloomThreshold;

extern float g_Time;
extern float g_DeltaTime;