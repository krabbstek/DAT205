#pragma once

#include "Common.h"

namespace core {

#define MEMORY_ALIGNMENT_SIZE 16

	class CORE_API Allocator
	{
	public:
		static void* Allocate(size_t blockSize);
		static void* AllocateDebug(size_t blockSize, const char* file, unsigned int line);

		static void* Reallocate(void* block, size_t blockSize);
		static void* ReallocateDebug(void* block, size_t blockSize, const char* file, unsigned int line);

		static void Free(void* block);
		static void FreeDebug(void* block, const char* file, unsigned int line);

		inline static size_t GetAllocationSize() { return s_AllocationSize; }
		inline static unsigned int GetNumAllocations() { return s_NumAllocations; }

		static void PrintStatus();

	private:
		static unsigned int s_NumAllocations;
		static unsigned int s_AllocationSize;
	};

}