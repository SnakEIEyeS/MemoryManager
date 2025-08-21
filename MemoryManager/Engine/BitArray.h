#pragma once

#include <stdint.h>

class HeapManager;

namespace Engine
{
	class BitArray
	{
	public:
		static BitArray * Create(size_t i_numBits, HeapManager * pHeapManager, bool iStartClear = false);
		~BitArray();

		void ClearAll(void);
		void SetAll(void);
		
		bool AreAllClear(void) const;
		bool AreAllSet(void) const;
		
		inline bool IsBitSet(const size_t & i_bitNumber) const;
		inline bool IsBitClear(const size_t & i_bitNumber) const;
		
		void SetBit(const size_t & i_bitNumber);
		void ClearBit(const size_t & i_bitNumber);

		bool GetFirstSetBit(size_t & o_bitNumber) const;
		bool GetFirstClearBit(size_t & o_bitNumber) const;
		
		//bool operator[](const size_t & i_index) const;


	private:
		BitArray(size_t i_numBits, bool iStartClear = false);	

#ifdef _WIN64
		uint64_t * m_pBits;
#else
		uint32_t * m_pBits;
#endif
		size_t m_numBits;
	};
}
