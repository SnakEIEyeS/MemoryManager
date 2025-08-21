//ToDo: 
#pragma once

#include "ConsolePrint.h"
#include "HeapManager.h"
#include "FixedSizeAllocator.h"


#ifdef _DEBUG
#ifdef _WIN32
#define WINDOWS() Engine::ConsolePrint("You are on Windows.\n\n")
#elif defined __APPLE__
#define MAC() Engine::ConsolePrint("You are on MacOS.\n\n")
#elif defined __UNIX__
#define UNIX() Engine::ConsolePrint("You are on a Unix build.\n\n")
#endif
#else
#define WINDOWS() void(0)
#define MAC() void(0)
#define UNIX() void(0)
#endif

#ifdef _DEBUG
#define DEBUGPRINT() Engine::ConsolePrint("This is the Debug build of the project.\n\n")
#define EMIT_LOC() (Engine::ConsolePrint("You have reached file:%s line:%d\n\n", __FILE__, __LINE__))
#define OUTSTANDINGALLOC() Engine::ConsolePrint("There are still outstanding allocations!")
#else
#define DEBUGPRINT() void(0)
#define EMIT_LOC() void(0)
#define OUTSTANDINGALLOC() void(0)
#endif


namespace Engine
{
	extern void EngineInit();

	extern void StartUp();

	extern HeapManager * CreateDefaultHeap(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);

	extern void InitFSASizes();

	extern HeapManager * CreateHeaps(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);

	extern FixedSizeAllocator * FindFixedSizeAllocator(const size_t & i_size);

	extern FixedSizeAllocator * FindFixedSizeAllocatorContains(void * i_ptr);

	extern void ResetHeap();

	extern void ShutDown();

	struct FSAInitData
	{
		size_t blockSize;
		size_t numBlocks;
	};

	//Data for Fixed Size Allocators
	const size_t FSAblockSizes[] = { 8, 16, 32, 48, 64, 80, 96, 112, 128 };
	//Keep number of blocks multiples of 64
	const size_t FSAnumBlocks[] = { 256, 256, 256, 256, 256, 256, 256, 256, 256 };

	const size_t numFSAs = sizeof(FSAblockSizes) / sizeof(FSAblockSizes[0]);

	static FixedSizeAllocator * FSAList[numFSAs];
	static bool bShutDown = false;

}
