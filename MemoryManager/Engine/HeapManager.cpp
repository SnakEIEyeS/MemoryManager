#include "HeapManager.h"

#include "assert.h"
#include <inttypes.h>
#include <iostream>
#include <memory.h>
#include <new>

#include "Allocators.h"
#include "Engine-init.h"
#include "BlockDescriptor.h"

#include "HeapManager-inl.h"


const unsigned int HeapManager::s_defaultAlignment = sizeof(void *);
const size_t HeapManager::s_minimumToLeave = 64;

const uint8_t fillValueAlloc = 0xbb;
const uint8_t fillValueFree = 0xdd;

HeapManager * HeapManager::create(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
{
	assert(i_sizeMemory > sizeof(HeapManager) + (sizeof(BlockDescriptor) * i_numDescriptors) + (s_defaultAlignment * 2));

	void * pHeapManagerMem = i_pMemory;
	i_pMemory = reinterpret_cast<uint8_t *>(i_pMemory) + sizeof(HeapManager);
	i_sizeMemory -= sizeof(HeapManager);

	return new (pHeapManagerMem) HeapManager(i_pMemory, i_sizeMemory, i_numDescriptors);
}

void HeapManager::destroy()
{
	if (m_pOutstandingMemory)
	{
		std::cout << "\nThere are still outstanding allocations in the HeapManager!\n";
		ShowOutstandingAllocations();
	}
}

HeapManager::HeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors) :
	m_sizeBase(i_sizeMemory), m_numDescriptors(i_numDescriptors), m_pOutstandingMemory(nullptr)
{
	assert(i_pMemory);
	assert(i_sizeMemory);
	assert(i_numDescriptors * sizeof(BlockDescriptor) < i_sizeMemory);

	m_pBase = reinterpret_cast<uint8_t *>(i_pMemory);

	m_pStartDescriptors = RoundUp<BlockDescriptor>(i_pMemory, __alignof(BlockDescriptor));
	InitializeFreeDescriptors(m_pStartDescriptors, m_numDescriptors);

	m_pAllocatable = RoundUp<uint8_t>(m_pStartDescriptors + m_numDescriptors, s_defaultAlignment);
	m_sizeAllocatable = m_pBase + i_sizeMemory - m_pAllocatable;

	m_pFreeMemory = GetFreeDescriptor();
	assert(m_pFreeMemory);

	m_pFreeMemory->m_pBase = m_pAllocatable;
	m_pFreeMemory->m_size = m_sizeAllocatable;
	m_pFreeMemory->m_pNextDesc = nullptr;
}

void * HeapManager::_alloc(size_t i_size, unsigned int i_alignment)
{
	assert(i_size);
	assert(IsPowerOfTwo(i_alignment));

	BlockDescriptor * pCurrent = m_pFreeMemory;

	while (pCurrent)
	{
		void * pUserMem = AllocateFromBlockEnd(*pCurrent, i_size, i_alignment);

		if (pUserMem)
			return pUserMem;

		pCurrent = pCurrent->m_pNextDesc;
	}

	return nullptr;
}

bool HeapManager::_free(void * i_ptr)
{
	if (i_ptr == nullptr)
		return true;

	uint8_t * pAdjustedPtr = reinterpret_cast<uint8_t *>(i_ptr);

	BlockDescriptor * pDesc = *RoundDown<BlockDescriptor *>(pAdjustedPtr - sizeof(BlockDescriptor *), __alignof(BlockDescriptor *));

	bool bIsValidDescriptor = IsValidDescriptorPtr(pDesc);
	assert(bIsValidDescriptor);

	if (!bIsValidDescriptor)
		return false;

	pDesc->m_sizeUserBlock = 0;

	memset(pDesc->m_pBase, fillValueFree, pDesc->m_size);

	{
		bool success = RemoveFromList(pDesc, m_pOutstandingMemory);
		assert(success);
	}

	pDesc->m_pUserBlock = NULL;
	pDesc->m_sizeUserBlock = 0;

	InsertInListMemorySequentially(pDesc, m_pFreeMemory);

	return true;
}

void HeapManager::collect()
{
	if (m_pFreeMemory)
	{
		BlockDescriptor * pCurrent = m_pFreeMemory;

		while (pCurrent->m_pNextDesc)
		{
			BlockDescriptor * pNext = pCurrent->m_pNextDesc;

			if (pCurrent->m_pBase + pCurrent->m_size == pNext->m_pBase)
			{
				pCurrent->m_size += pNext->m_size;
				pCurrent->m_pNextDesc = pNext->m_pNextDesc;

				ReturnFreeDescriptor(pNext);
			}
			else
				pCurrent = pCurrent->m_pNextDesc;
		}
	}
}

void * HeapManager::AllocateFromBlockEnd(BlockDescriptor & i_Block, size_t i_size, unsigned int i_alignment)
{
	size_t offsetFromEnd = i_size;

	uint8_t * pUserMemory = RoundDown<uint8_t>(i_Block.m_pBase + i_Block.m_size - offsetFromEnd, i_alignment);

	uint8_t * pBlockStart = pUserMemory;
	pBlockStart = RoundDown<uint8_t>(pBlockStart - sizeof(BlockDescriptor *), __alignof(BlockDescriptor *));

	if (pBlockStart < i_Block.m_pBase)
		return nullptr;

	BlockDescriptor * pDesc = nullptr;

	if ((pBlockStart - i_Block.m_pBase) < s_minimumToLeave)
	{
		RemoveFromList(&i_Block, m_pFreeMemory);
		pDesc = &i_Block;
	}
	else
	{
		pDesc = GetFreeDescriptor();
		assert(pDesc && "Out of Block Descriptors!");

		if (!pDesc)
			return nullptr;

		pDesc->m_size = i_Block.m_pBase + i_Block.m_size - pBlockStart;
		pDesc->m_pBase = pBlockStart;
		pDesc->m_pNextDesc = nullptr;

		i_Block.m_size = pBlockStart - i_Block.m_pBase;
	}

	assert(pDesc);

	memset(pUserMemory, fillValueAlloc, i_size);

	BlockDescriptor ** pCachedPtr = reinterpret_cast<BlockDescriptor **>(pBlockStart);
	*pCachedPtr = pDesc;

	pDesc->m_sizeUserBlock = i_size;
	pDesc->m_pUserBlock = pUserMemory;
	InsertInListMemorySequentially(pDesc, m_pOutstandingMemory);

	return pUserMemory;
}

size_t HeapManager::getLargestFreeBlock() const
{
	size_t largestBlock = 0;

	BlockDescriptor * pCurrent = m_pFreeMemory;

	while (pCurrent)
	{
		if (pCurrent->m_size > largestBlock)
			largestBlock = pCurrent->m_size;

		pCurrent = pCurrent->m_pNextDesc;
	}

	return largestBlock;
}

size_t HeapManager::getTotalFreeMemory() const
{
	size_t total = 0;

	BlockDescriptor * pCurrent = m_pFreeMemory;

	while (pCurrent)
	{
		total += pCurrent->m_size;
		pCurrent = pCurrent->m_pNextDesc;
	}

	return total;
}

unsigned int HeapManager::ListItems(const BlockDescriptor * i_pDesc)
{
	unsigned int count = 0;

	while (i_pDesc)
	{
		count++;
		i_pDesc = i_pDesc->m_pNextDesc;
	}

	return count;
}

void HeapManager::InsertInListMemorySequentially(BlockDescriptor * i_pDesc, BlockDescriptor * & io_pList)
{
	BlockDescriptor ** ppCurrent = &io_pList;

	while (*ppCurrent && (*ppCurrent)->m_pBase < i_pDesc->m_pBase)
		ppCurrent = &(*ppCurrent)->m_pNextDesc;

	i_pDesc->m_pNextDesc = *ppCurrent;
	*ppCurrent = i_pDesc;
}

bool HeapManager::RemoveFromList(BlockDescriptor * i_pDesc, BlockDescriptor * & io_pList)
{
	BlockDescriptor ** ppCurrent = &io_pList;

	while (*ppCurrent)
	{
		if (*ppCurrent == i_pDesc)
		{
			*ppCurrent = i_pDesc->m_pNextDesc;
			i_pDesc->m_pNextDesc = nullptr;
			return true;
		}

		ppCurrent = &(*ppCurrent)->m_pNextDesc;
	}

	return false;
}

bool HeapManager::IsAllocated(void * i_ptr) const
{
	uint8_t * pAdjustedPtr = reinterpret_cast<uint8_t *>(i_ptr);

	BlockDescriptor * pDesc = *RoundDown<BlockDescriptor *>(pAdjustedPtr - sizeof(BlockDescriptor *), __alignof(BlockDescriptor *));

	return IsValidDescriptorPtr(pDesc);
}

void HeapManager::InitializeFreeDescriptors(BlockDescriptor * i_pFirstDescriptor, size_t i_numDescriptors)
{
	m_pFreeDescriptors = i_pFirstDescriptor;

	for (size_t i = 0; i < (i_numDescriptors - 1); i++)
	{
		i_pFirstDescriptor->m_size = 0;
		i_pFirstDescriptor->m_pBase = nullptr;

		i_pFirstDescriptor->m_pNextDesc = i_pFirstDescriptor + 1;

		i_pFirstDescriptor++;
	}

	i_pFirstDescriptor->m_pNextDesc = nullptr;

#ifdef _DEBUG
	size_t count = 0;
	BlockDescriptor * pCurrent = m_pFreeDescriptors;

	while (pCurrent && (count < i_numDescriptors))
	{
		count++;
		pCurrent = pCurrent->m_pNextDesc;
	}

	assert((count == i_numDescriptors) && (pCurrent == nullptr));
#endif
}

BlockDescriptor * HeapManager::GetFreeDescriptor(void)
{
	BlockDescriptor * pReturnPtr = m_pFreeDescriptors;

	if (m_pFreeDescriptors)
		m_pFreeDescriptors = pReturnPtr->m_pNextDesc;

	return pReturnPtr;
}

void HeapManager::ReturnFreeDescriptor(BlockDescriptor * i_pDesc)
{
#ifdef _DEBUG
	i_pDesc->m_pBase = nullptr;
	i_pDesc->m_size = 0;
#endif _DEBUG

	i_pDesc->m_pNextDesc = m_pFreeDescriptors;

	m_pFreeDescriptors = i_pDesc;
}

bool HeapManager::IsValidDescriptorPtr(const BlockDescriptor * i_pDesc) const
{
	if (i_pDesc < m_pStartDescriptors)
		return false;

	if (i_pDesc >= (m_pStartDescriptors + m_numDescriptors))
		return false;

	if ((reinterpret_cast<uintptr_t>(i_pDesc) - reinterpret_cast<uintptr_t>(m_pStartDescriptors)) % sizeof(BlockDescriptor) != 0)
		return false;

	return true;
}

#ifdef __64BIT
#define POINTERCHARS 16
#elif defined __32BIT
#define POINTERCHARS 8
#endif


void HeapManager::ShowFreeBlocks() const
{
	BlockDescriptor * pCurrent = m_pFreeMemory;
	printf("FreeBlocks:\n");
	if (pCurrent)
	{
		while (pCurrent)
		{
			std::cout << "Free Block: " << (uintptr_t)pCurrent->m_pBase << " Size: " << pCurrent->m_size << "\n";
			pCurrent = pCurrent->m_pNextDesc;
		}
	}
	else
		printf("\tNo free blocks.\n");
}

void HeapManager::ShowOutstandingAllocations() const
{
	BlockDescriptor * pCurrent = m_pOutstandingMemory;
	printf("Outstanding allocations:\n");
	if (pCurrent)
	{
		while (pCurrent)
		{
			std::cout << "BlockStart: " << (uintptr_t)pCurrent->m_pBase << " Size: " << pCurrent->m_size << "\n";
			std::cout << "User Block: " << (uintptr_t)pCurrent->m_pUserBlock << " User BlockSize: " << pCurrent->m_sizeUserBlock << "\n";
			pCurrent = pCurrent->m_pNextDesc;
		}
	}
}
