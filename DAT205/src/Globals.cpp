#include "graphics/Light.h"

#include "ConstExpr.h"

Light g_GlobalLight = { vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1000.0f, 1000.0f, 1000.0f, 1.0f) };

float g_SSAOKernelSize = 0.3f;
float g_SSAORadius = 10.0f;
int g_SSAOBlurSampleSize = 5;
float g_SSAOBilateralBlurSigma = 1.0f;
float g_SSAODepthSigma = 1.0f;

float g_EnvironmentMultiplier = 1.0f;

float g_MotionBlurVelocityScale = 1.0f;

float g_BloomAlpha = 1.0f;
float g_BloomThreshold = 1.0f;

float g_Time;
float g_DeltaTime = 0.0f;