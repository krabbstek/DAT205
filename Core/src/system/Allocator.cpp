#include "pch.h"

#include "Allocator.h"

namespace core {

	unsigned int Allocator::s_NumAllocations = 0;
	unsigned int Allocator::s_AllocationSize = 0;


	void* Allocator::Allocate(size_t blockSize)
	{
		size_t memorySize = blockSize + MEMORY_ALIGNMENT_SIZE;

		char* block = (char*)_aligned_malloc(memorySize, MEMORY_ALIGNMENT_SIZE);
		*((size_t*)block) = blockSize;

		block += MEMORY_ALIGNMENT_SIZE;
		memset(block, 0, blockSize);

		s_NumAllocations++;
		s_AllocationSize += blockSize;

		return (void*)block;
	}

	void* Allocator::AllocateDebug(size_t blockSize, const char* file, unsigned int line)
	{
		if (blockSize >= 1024)
			CORE_TRACE("Memory allocation of {} bytes in {}, line {}.", blockSize, file, line);

		return Allocate(blockSize);
	}


	void* Allocator::Reallocate(void* block, size_t blockSize)
	{
		size_t memorySize = blockSize + MEMORY_ALIGNMENT_SIZE;

		block = (char*)block - MEMORY_ALIGNMENT_SIZE;
		size_t oldBlockSize = *((size_t*)block);

		char* newBlock = (char*)_aligned_realloc(block, memorySize, MEMORY_ALIGNMENT_SIZE);
		*((size_t*)newBlock) = blockSize;

		newBlock += MEMORY_ALIGNMENT_SIZE;
		if (blockSize > oldBlockSize)
		{
			block = newBlock + oldBlockSize;
			memset(block, 0, blockSize - oldBlockSize);
		}

		s_AllocationSize += blockSize - oldBlockSize;

		return (void*)newBlock;
	}

	void* Allocator::ReallocateDebug(void* block, size_t blockSize, const char* file, unsigned int line)
	{
		if (blockSize >= 1024)
			CORE_TRACE("Memory re-allocation of {} bytes in {}, line {}.", blockSize, file, line);

		return Reallocate(block, blockSize);
	}


	void Allocator::Free(void* block)
	{
		if (!block)
		{
#ifdef CORE_WARN_DELETE_NULLPTR
			CORE_WARN("Allocator::Free(void* block) called with nullptr!");
#endif
			return;
		}

		block = (char*)block - MEMORY_ALIGNMENT_SIZE;
		size_t blockSize = *((size_t*)block);

		_aligned_free(block);

		s_NumAllocations--;
		s_AllocationSize -= blockSize;
	}

	void Allocator::FreeDebug(void* block, const char* file, unsigned int line)
	{
		char* ptr = (char*)block - MEMORY_ALIGNMENT_SIZE;
		size_t blockSize = *((size_t*)ptr);

		if (blockSize >= 1024)
			CORE_TRACE("Memory({} bytes) freed in {}, line {}.", blockSize, file, line);

		return Free(block);
	}


	void Allocator::PrintStatus()
	{
		CORE_INFO("Memory:");
		CORE_INFO("Number of allocations: {}", s_NumAllocations);
		CORE_INFO("Total allocation size: {} bytes", s_AllocationSize);
	}

}