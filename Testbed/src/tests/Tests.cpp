#include "Tests.h"

TestEntry testEntries[] =
{
	{ "Clear Color", ClearColorTest::Create },
	{ "Dissolve 2D", Dissolve2DTest::Create },
	{ NULL, NULL },
};

unsigned int numTestEntries = 0;