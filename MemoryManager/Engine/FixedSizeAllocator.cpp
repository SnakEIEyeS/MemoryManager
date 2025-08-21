//ToDo:

#include "FixedSizeAllocator.h"

#include "assert.h"
#include <iostream>
#include <memory.h>
#include "stdint.h"

#include "Allocators.h"
#include "Engine-init.h"
#include "HeapManager.h"

//Alignment for Fixed Size Allocator's creation
const unsigned int defaultAlignment = 4;

//alloc and free fill values
const uint8_t fillValueAlloc = 0xbb;
const uint8_t fillValueFree = 0xdd;

namespace Engine
{
	FixedSizeAllocator * FixedSizeAllocator::CreateFSA(size_t i_blockSize, size_t i_numBlocks, HeapManager * pHeapManager)
	{
		BitArray * pBitArr = BitArray::Create(i_numBlocks,pHeapManager, false);
		assert(pBitArr);
		void * pFSAMem = pHeapManager->_alloc(i_blockSize * i_numBlocks + sizeof(FixedSizeAllocator), defaultAlignment);

		void * pFSAFirstMem = reinterpret_cast<uint8_t *>(pFSAMem) + sizeof(FixedSizeAllocator);

		return new (pFSAMem) FixedSizeAllocator(pFSAFirstMem, i_blockSize, i_numBlocks, pBitArr);
	}

	FixedSizeAllocator::FixedSizeAllocator(void * i_pMemory, size_t i_blockSize, size_t i_numBlocks, BitArray * i_bitArray) : 
		m_blockSize(i_blockSize), m_numBlocks(i_numBlocks)
	{
		assert(i_pMemory);
		m_pBase = i_pMemory;
		m_pBitArray = i_bitArray;

		m_totalMemory = m_blockSize * m_numBlocks;

		//Set all bits of BitArray for safety
		this->m_pBitArray->SetAll();
	}

	FixedSizeAllocator::~FixedSizeAllocator()
	{
		if(m_pBitArray)
			delete m_pBitArray;	
	}

	void FixedSizeAllocator::destroy()
	{
		if (!m_pBitArray->AreAllSet())
		{
#ifdef _DEBUG
			std::cout << "\nThere are outstanding allocations in the FSA - Size: " << m_blockSize << "\n";
#endif
		}

		return;
	}

	void * FixedSizeAllocator::alloc()
	{
		size_t i_firstAvailableBit;

		if (m_pBitArray->GetFirstSetBit(i_firstAvailableBit))
		{
			//Clear corresponding bit to denote block is allocated
			m_pBitArray->ClearBit(i_firstAvailableBit);

			//std::cout << "Allocating from FSA.\n";

			void * pReturn = reinterpret_cast<uint8_t *>(m_pBase) + (i_firstAvailableBit * m_blockSize);

			//Validate if the obtained pointer is within the FSA and a valid block
			bool bIsValidAlloc = IsValidFSAPtr(pReturn);
			assert(bIsValidAlloc);

			memset(pReturn, fillValueAlloc, m_blockSize);

			return pReturn;
		}
		else
			return nullptr;
	}

	bool FixedSizeAllocator::free(void * i_ptr)
	{
		if (i_ptr == nullptr)
			return true;

		//Check if i_ptr is valid and belongs to this FSA.
		bool bIsValid = IsValidFSAPtr(i_ptr);
		assert(bIsValid);

		if(bIsValid)
		{
			//Find and set bit corresponding to the pointer in FSA for future allocation
			size_t i_toSetBit = (reinterpret_cast<uintptr_t>(i_ptr) - reinterpret_cast<uintptr_t>(m_pBase)) / m_blockSize;
			m_pBitArray->SetBit(i_toSetBit);
			//i_ptr = nullptr;

			//std::cout << "Freeing from FSA.\n";

			memset(i_ptr, fillValueFree, m_blockSize);

			return true;
		}

		return false;
	}

	bool FixedSizeAllocator::IsValidFSAPtr(void * i_ptr) const
	{
		if (i_ptr >= m_pBase && i_ptr < (reinterpret_cast<uint8_t *>(m_pBase) + m_totalMemory) && ((reinterpret_cast<uintptr_t>(i_ptr) - reinterpret_cast<uintptr_t>(m_pBase)) % m_blockSize) == 0)
			return true;

		return false;
	}

	size_t FixedSizeAllocator::getBlockSize() const
	{
		return m_blockSize;
	}
}