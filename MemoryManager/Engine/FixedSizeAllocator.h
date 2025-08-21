#pragma once

#include "BitArray.h"

namespace Engine
{
	class FixedSizeAllocator
	{
	public:
		static FixedSizeAllocator * CreateFSA(size_t i_blockSize, size_t i_numBlocks, HeapManager * pHeapManager);
		~FixedSizeAllocator();
		
		void destroy();

		void * alloc();

		bool free(void * i_ptr);

		bool IsValidFSAPtr(void * i_ptr) const;

		size_t getBlockSize() const;


	private:
		FixedSizeAllocator(void * i_pMemory, size_t i_blockSize, size_t i_numBlocks, BitArray * i_bitArray);

		void * m_pBase;
		size_t m_blockSize;
		size_t m_numBlocks;
		size_t m_totalMemory;
		BitArray * m_pBitArray;
	};
}