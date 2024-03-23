#pragma once

#include "Test.h"

#include "ClearColorTest/ClearColorTest.h"
#include "Dissolve2DTest/Dissolve2DTest.h"
#include "DissolveBloom2DTest/DissolveBloom2DTest.h"
#include "GaussianBlurTest/GaussianBlurTest.h"

struct TestEntry
{
	const char* name;
	Test* (*CreateFcn)();
};

extern TestEntry testEntries[];
extern unsigned int numTestEntries;