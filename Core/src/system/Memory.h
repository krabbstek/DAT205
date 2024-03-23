#pragma once

#include "Common.h"

#include "Allocator.h"

#ifdef DEBUG
#define cnew new(__FILE__, __LINE__)
#else
#define cnew new
#endif

#pragma warning(disable : 4595)

inline void* operator new(size_t blockSize)
{
	return core::Allocator::Allocate(blockSize);
}

inline void* operator new(size_t blockSize, const char* file, unsigned int line)
{
	return core::Allocator::AllocateDebug(blockSize, file, line);
}

inline void* operator new[](size_t blockSize)
{
	return core::Allocator::Allocate(blockSize);
}

inline void* operator new[](size_t blockSize, const char* file, unsigned int line)
{
	return core::Allocator::AllocateDebug(blockSize, file, line);
}

inline void operator delete(void* block)
{
	core::Allocator::Free(block);
}

inline void operator delete(void* block, const char* file, unsigned int line)
{
	core::Allocator::FreeDebug(block, file, line);
}

inline void operator delete[](void* block)
{
	core::Allocator::Free(block);
}

inline void operator delete[](void* block, const char* file, unsigned int line)
{
	core::Allocator::FreeDebug(block, file, line);
}

#pragma warning(default : 4595)