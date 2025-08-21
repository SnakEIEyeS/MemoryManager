#pragma once
#include "HeapManager.h"

#include <vector>

static bool bDefaultHeapInit;
static bool bFSAListInitialized;
static HeapManager * pDefaultHeap;

void AddAllocator(HeapManager * pHeapManager);

HeapManager * getDefaultHeap();

void * operator new(size_t size);
void operator delete(void * i_ptr);

void * operator new[](size_t size);
void operator delete[](void * i_ptr);

enum allocAlign
{
	ALLOC_ALIGN_DEFAULT,
	ALLOC_ALIGN_8 = 8,
	ALLOC_ALIGN_16 = 16,
	ALLOC_ALIGN_32 = 32
};

void * operator new(size_t size, HeapManager * pHeap, allocAlign alignment);
void operator delete(void * i_ptr, HeapManager * pHeap, allocAlign alignment);

void * operator new(size_t size, HeapManager * pHeap, unsigned int alignment);
void operator delete(void * i_ptr, HeapManager * pHeap, unsigned int alignment);

void * operator new(size_t size, HeapManager * pHeap);
void operator delete(void * i_ptr, HeapManager * pHeap);

