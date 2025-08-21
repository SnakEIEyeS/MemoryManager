//ToDo: 

#include "BitArray.h"

#include <assert.h>
#include <intrin.h>
#include <iostream>
#include <memory.h>
#include <stdint.h>

#include "Allocators.h"

#ifdef _WIN64
const size_t bitsPerSegment = 64;
#else
const size_t bitsPerSegment = 32;
#endif

const size_t bitsPerByte = 8;

namespace Engine
{
	BitArray * BitArray::Create(size_t i_numBits, HeapManager * pHeapManager, bool iStartClear)
	{
		assert(i_numBits);
		assert(pHeapManager);
		return new (pHeapManager) BitArray(i_numBits, iStartClear);
	}

	BitArray::BitArray(size_t i_numBits, bool iStartClear) : m_numBits(i_numBits)
	{
		//const size_t bitsPerSegment = 8;

#ifdef _WIN64
		m_pBits = new uint64_t[i_numBits / bitsPerSegment];
#else
		m_pBits = new uint32_t[i_numBits / bitsPerSegment];
#endif
		assert(m_pBits);

		memset(m_pBits, iStartClear ? 0x00 : 0xFF, i_numBits / bitsPerByte);
		//std::cout << m_numBits;
	}

	BitArray::~BitArray()
	{
		if(m_pBits)
			delete[] m_pBits;
	}

	void BitArray::ClearAll(void)
	{
		void * pBitsPtr = m_pBits;
		const size_t lengthBytes = this->m_numBits / bitsPerByte;
		memset(pBitsPtr, 0x00, lengthBytes);
	}

	void BitArray::SetAll(void)
	{
		void * pBitsPtr = m_pBits;
		const size_t lengthBytes = this->m_numBits / bitsPerByte;
		//std::cout << "\t" << lengthBytes << "\n";
		memset(pBitsPtr, 0xFF, lengthBytes);
	}


	bool BitArray::AreAllClear(void) const
	{
#if defined(_WIN64)
		uint64_t * mask = m_pBits;
#else
		uint32_t * mask = m_pBits;
#endif

		unsigned int checkedBitCount = 0;

		//Keep advancing until a set bit is found or end is reached
#if defined(_WIN64)
		while (mask[0] == 0x0000000000000000 && checkedBitCount < m_numBits)
		{
			checkedBitCount += bitsPerSegment;
			mask++;
		}
#else
		while (mask[0] == 0x00000000 && checkedBitCount < m_numBits)
		{
			checkedBitCount += bitsPerSegment;
			mask++;
		}
#endif

		if (checkedBitCount == m_numBits)
			return true;

		return false;
	}

	bool BitArray::AreAllSet(void) const
	{	
#if defined(_WIN64)
		uint64_t * mask = m_pBits;
#else
		uint32_t * mask = m_pBits;
#endif

		unsigned int checkedBitCount = 0;

		//Keep advancing until a clear bit is found or end is reached
#if defined(_WIN64)
		while (mask[0] == 0xFFFFFFFFFFFFFFFF && checkedBitCount < m_numBits)
		{
			checkedBitCount += bitsPerSegment;
			mask++;
		}
#else
		while (mask[0] == 0xFFFFFFFF && checkedBitCount < m_numBits)
		{
			checkedBitCount += bitsPerSegment;
			mask++;
		}
#endif

		if (checkedBitCount == m_numBits)
			return true;

		return false;
	}


	bool BitArray::GetFirstClearBit(size_t & o_bitNumber) const
	{
		o_bitNumber = 0;

#if defined(_WIN64)
		uint64_t * mask = m_pBits;
#else
		uint32_t * mask = m_pBits;
#endif

		unsigned int checkedBitCount = 0;

		//Skip segments that do not contain a clear bit at all
#if defined(_WIN64)
		while (mask[0] == 0xFFFFFFFFFFFFFFFF && checkedBitCount < m_numBits)
		{
			checkedBitCount += bitsPerSegment;
			o_bitNumber += bitsPerSegment;
			mask++;
		}
#else
		while (mask[0] == 0xFFFFFFFF && checkedBitCount < m_numBits)
		{
			checkedBitCount += bitsPerSegment;
			o_bitNumber += bitsPerSegment;
			mask++;
		}
#endif

		if (checkedBitCount == m_numBits)
		{
			o_bitNumber = 0;
			return false;
		}
		else
		{
			//Next segment has to have a clear bit
			unsigned long index = 0;
#if defined(_WIN64)
			unsigned char clearBit = _BitScanForward64(&index, ~mask[0]);
#else
			unsigned char clearBit = _BitScanForward(&index, ~mask[0]);
#endif

			//Safeguarding with if statement
			if (clearBit)
			{
				o_bitNumber += index;
				return true;
			}

			//Safeguarding
			o_bitNumber = 0;
			return false;
		}
	}
	
	bool BitArray::GetFirstSetBit(size_t & o_bitNumber) const
	{
		o_bitNumber = 0;

#if defined(_WIN64)
		uint64_t * mask = m_pBits;
#else
		uint32_t * mask = m_pBits;
#endif

		unsigned int checkedBitCount = 0;

		//Skip segments that do not contain a set bit at all
#if defined(_WIN64)
		while (mask[0] == 0x0000000000000000 && checkedBitCount < m_numBits)
		{
			checkedBitCount += bitsPerSegment;
			o_bitNumber += bitsPerSegment;
			mask++;
		}
#else
		while (mask[0] == 0x00000000 && checkedBitCount < m_numBits)
		{
			checkedBitCount += bitsPerSegment;
			o_bitNumber += bitsPerSegment;
			mask++;
		}
#endif

		if (checkedBitCount == m_numBits)
		{
			o_bitNumber = 0;
			return false;
		}
		else
		{
			//Next segment has to have a set bit
			unsigned long index = 0;
#if defined(_WIN64)
			unsigned char setBit = _BitScanForward64(&index, mask[0]);
#else
			unsigned char setBit = _BitScanForward(&index, mask[0]);
#endif

			//Safeguarding with if statement
			if (setBit)
			{
				o_bitNumber += index;
				return true;
			}

			//Safeguarding
			o_bitNumber = 0;
			return false;
		}
	}
	
	inline bool BitArray::IsBitClear(const size_t & i_bitNumber) const
	{
		assert(i_bitNumber < m_numBits);

		//Divide and take modulus with bitsPerSegment to get exact location in array.
		//i_bitNumber starts at 0.
		size_t index = i_bitNumber / bitsPerSegment;
		size_t bitNumInSeg = i_bitNumber % bitsPerSegment;

#if defined(_WIN64)
		unsigned int result = m_pBits[index] & (1ULL << bitNumInSeg);
#else
		unsigned int result = m_pBits[index] & (1 << bitNumInSeg);
#endif

		if (result > 0)
			return false;

		else
			return true;
	}
	
	inline bool BitArray::IsBitSet(const size_t & i_bitNumber) const
	{
		assert(i_bitNumber < m_numBits);

		//Divide and take modulus with bitsPerSegment to get exact location in array.
		//i_bitNumber starts at 0.
		size_t index = i_bitNumber / bitsPerSegment;
		size_t bitNumInSeg = i_bitNumber % bitsPerSegment;

#if defined(_WIN64)
		unsigned int result = m_pBits[index] & (1ULL << bitNumInSeg);
#else
		unsigned int result = m_pBits[index] & (1 << bitNumInSeg);
#endif

		if (result > 0)
			return true;

		else
			return false;
	}

	void BitArray::ClearBit(const size_t & i_bitNumber)
	{
		assert(i_bitNumber < m_numBits);

		//Divide and take modulus with bitsPerSegment to get exact location in array.
		//i_bitNumber starts at 0.
		size_t index = i_bitNumber / bitsPerSegment;
		size_t bitNumInSeg = i_bitNumber % bitsPerSegment;

#if defined(_WIN64)
		m_pBits[index] = m_pBits[index] & ~(1ULL << bitNumInSeg);
#else
		m_pBits[index] = m_pBits[index] & ~(1 << bitNumInSeg);
#endif
	}
	
	void BitArray::SetBit(const size_t & i_bitNumber)
	{
		assert(i_bitNumber < m_numBits);

		//Divide and take modulus with bitsPerSegment to get exact location in array.
		//i_bitNumber starts at 0.
		size_t index = i_bitNumber / bitsPerSegment;
		size_t bitNumInSeg = i_bitNumber % bitsPerSegment;

#if defined(_WIN64)
		m_pBits[index] = m_pBits[index] | (1ULL << bitNumInSeg);
#else
		m_pBits[index] = m_pBits[index] | (1 << bitNumInSeg);
#endif
	}

/*
	bool BitArray::operator[](const size_t & index) const
	{
		assert(index < m_numBits && "Index out of bounds.");
		if (index >= m_numBits)
		{
			std::cout << "Index out of bounds.\n";
			return false;
		}

		return true;
	}
	*/
}