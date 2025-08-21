#include "Allocators.h"

#include <iostream>

#include "Engine-init.h"
#include "FixedSizeAllocator.h"
#include "HeapManager.h"

//Set the default Heap Manager
void AddAllocator(HeapManager * pHeapManager)
{
	pDefaultHeap = pHeapManager;
}

//Return the default Heap Manager
HeapManager * getDefaultHeap()
{
	return pDefaultHeap;
}

//Default operator new & delete
void * operator new(size_t size)
{
	//printf("Normal operator new overridden.\n");

	void * pReturnPtr = nullptr;

	Engine::FixedSizeAllocator * pFSA = Engine::FindFixedSizeAllocator(size);

	if (pFSA)
		pReturnPtr = pFSA->alloc();

	if (pReturnPtr == nullptr)
		pReturnPtr = pDefaultHeap->_alloc(size);
	
	if (pReturnPtr == nullptr)
		std::cout << "No more memory to allocate from!";

	return pReturnPtr;
}

void operator delete(void * i_ptr)
{
	//printf("Normal operator delete overridden.\n");

	if (i_ptr)
	{
		Engine::FixedSizeAllocator * pFSA = Engine::FindFixedSizeAllocatorContains(i_ptr);

		if (pFSA && pFSA->free(i_ptr))
		{
			return;
		}
		
		if (pDefaultHeap->_free(i_ptr))
			return;
	}
	//_aligned_free(i_ptr);
}

//Operator array new & delete
void * operator new[](size_t size)
{
	//printf("Running operator new[] overridden.\n");
	void * pReturnPtr = nullptr;

	Engine::FixedSizeAllocator * pFSA = Engine::FindFixedSizeAllocator(size);

	if (pFSA)
		pReturnPtr = pFSA->alloc();

	if (pReturnPtr == nullptr)
		pReturnPtr = pDefaultHeap->_alloc(size);
		
	return pReturnPtr;

	//return _aligned_malloc(size, 4);
}

void operator delete[](void * i_ptr)
{
	//printf("Running operator delete[] overridden.\n");
	if (i_ptr)
	{
		Engine::FixedSizeAllocator * pFSA = Engine::FindFixedSizeAllocatorContains(i_ptr);

		if (pFSA && pFSA->free(i_ptr))
		{
			return;
		}
		
		if (pDefaultHeap->_free(i_ptr))
			return;
	}
	//_aligned_free(i_ptr);
}

//Operator new & delete with alignment and specific HeapManager
void * operator new(size_t size, HeapManager * pHeap, allocAlign alignment)
{
	//printf("Running new with enum!\n");

	switch (alignment)
	{
	case ALLOC_ALIGN_8:
	case ALLOC_ALIGN_16:
	case ALLOC_ALIGN_32:
		return pHeap->_alloc(size, alignment);
		break;

	default:
		return pHeap->_alloc(size, 4);
		break;
	}
}

void operator delete(void * i_ptr, HeapManager * pHeap, allocAlign alignment)
{
	//printf("Running delete with enum!\n");
	pHeap->_free(i_ptr);
}


void * operator new(size_t size, HeapManager * pHeap, unsigned int alignment)
{
	//printf("Running new with 3 parameters!\n");
	return pHeap->_alloc(size, alignment);
}

void operator delete(void * i_ptr, HeapManager * pHeap, unsigned int alignment)
{
	//printf("Running delete with 3 parameters!\n");
	pHeap->_free(i_ptr);
}

//Operator new & delete with specific HeapManager
void * operator new(size_t size, HeapManager * pHeap)
{
	//printf("Running new with 2 parameters!\n");
	return pHeap->_alloc(size);
}

void operator delete(void * i_ptr, HeapManager * pHeap)
{
	//printf("Running delete with 2 parameters!\n");
	pHeap->_free(i_ptr);
}

