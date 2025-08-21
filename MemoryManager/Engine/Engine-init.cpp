//ToDo:
#include "Engine-init.h"

#include "assert.h"
#include <iostream>
#include <malloc.h>

#include "Allocators.h"
#include "FixedSizeAllocator.h"
#include "HeapManager.h"


namespace Engine
{
	const size_t defaultSizeMemory = 1024 * 1024;
	const unsigned int defaultNumDescriptors = 2048;

	FSAInitData FSASizes[numFSAs];

	//Initialize the Engine
	void EngineInit()
	{
		std::cout << "Engine initialized.\n";

		//Initialize array of FSA sizes
		InitFSASizes();
	}

	void StartUp()
	{
		bDefaultHeapInit = false;
		bFSAListInitialized = false;

		void * i_pMemory = _aligned_malloc(defaultSizeMemory, 4);
		HeapManager * pDefaultHeap = CreateHeaps(i_pMemory, defaultSizeMemory, defaultNumDescriptors);

		return;
	}

	//Creating the dynamic Heap Manager
	HeapManager * CreateDefaultHeap(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
	{
		assert(i_pMemory);
		assert(i_sizeMemory);
		assert(i_numDescriptors);
		EMIT_LOC();
		HeapManager * defaultHeapMgr = HeapManager::create(i_pMemory, i_sizeMemory, i_numDescriptors);
		EMIT_LOC();
		assert(defaultHeapMgr);

		if (!bDefaultHeapInit)
		{
			bDefaultHeapInit = true;
			AddAllocator(defaultHeapMgr);
		}

		return defaultHeapMgr;
	}

	void InitFSASizes()
	{
		for (unsigned int i = 0; i < numFSAs; i++)
		{
			FSASizes[i].blockSize = FSAblockSizes[i];
			FSASizes[i].numBlocks = FSAnumBlocks[i];
		}
	}

	//Create a dynamic Heap Manager and Fixed Size Allocators
	HeapManager * CreateHeaps(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
	{
		EMIT_LOC();
		HeapManager * pHeapMgr = CreateDefaultHeap(i_pMemory, i_sizeMemory, i_numDescriptors);
		assert(pHeapMgr);

		for (size_t i = 0; i < numFSAs; i++)
		{
			FSAList[i] = FixedSizeAllocator::CreateFSA(FSASizes[i].blockSize, FSASizes[i].numBlocks, pHeapMgr);
		}

		bFSAListInitialized = true;

		EMIT_LOC();
		std::cout << "\nHeaps Created.\n";
		return pHeapMgr;
	}

	//Find an FSA with blocks of the appropriate size
	FixedSizeAllocator * FindFixedSizeAllocator(const size_t & i_size)
	{
		if (bFSAListInitialized)
		{
			if (i_size <= FSAList[0]->getBlockSize())
				return FSAList[0];

			for (unsigned int i = 1; i < numFSAs; i++)
			{
				if (i_size <= FSAList[i]->getBlockSize() && i_size > FSAList[i - 1]->getBlockSize())
					return FSAList[i];
			}
		}

		return nullptr;
	}

	//Find an FSA that contains a pointer to memory
	FixedSizeAllocator * FindFixedSizeAllocatorContains(void * i_ptr)
	{
		if (bFSAListInitialized && !bShutDown)
		{
			for (unsigned int i = 0; i < numFSAs; i++)
			{
				if (FSAList[i] && FSAList[i]->IsValidFSAPtr(i_ptr))
					return FSAList[i];
			}
		}

		//If none found return null
		return nullptr;
	}

	void ResetHeap()
	{
		if (bDefaultHeapInit)
			bDefaultHeapInit = false;

		if (bFSAListInitialized)
			bFSAListInitialized = false;

		return;
	}

	void ShutDown()
	{
		bShutDown = true;
		bFSAListInitialized = false;

		HeapManager * pHeapFreeFSA = getDefaultHeap();

		for (size_t i = 0; i < numFSAs; i++)
		{
			FSAList[i]->destroy();
			//delete FSAList[i];

			bool bFSAFree = pHeapFreeFSA->_free(FSAList[i]);
			assert(bFSAFree);

			FSAList[i] = nullptr;
		}

		std::cout << "Engine shutdown completed.\n";

		return;
	}
}