#pragma once

#include "assert.h"

//#include "HeapManager.h"

inline bool IsPowerOfTwo(unsigned int i_Num)
{
	return i_Num && !(i_Num & (i_Num - 1));;
}

template<typename T>
inline T* RoundUp(void * i_memAddr, unsigned int i_alignment)
{
	assert(i_alignment);
	assert(IsPowerOfTwo(i_alignment));
	return reinterpret_cast<T *>((reinterpret_cast<uintptr_t>(i_memAddr) + (i_alignment - 1)) & ~uintptr_t(i_alignment - 1));
}

template<typename T>
inline T* RoundDown(void * i_memAddr, unsigned int i_alignment)
{
	assert(i_alignment);
	assert(IsPowerOfTwo(i_alignment));
	return reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(i_memAddr) & ~uintptr_t((i_alignment - 1)));
}


		inline void * HeapManager::_alloc(size_t i_size)
		{
			return _alloc(i_size, s_defaultAlignment);
		}

		inline bool HeapManager::Contains(void * i_ptr) const
		{
			return (i_ptr >= m_pAllocatable) && (i_ptr < (m_pAllocatable + m_sizeBase));
		}
	

template<typename T>
inline T AlignBack(int i_memAddr, int i_alignment)
{
	assert(i_alignment);
	assert(IsPowerOfTwo(i_alignment));
	return ((i_memAddr) & ~(i_alignment - 1));
}

template<typename T>
inline T AlignForward(int i_memAddr, int i_alignment)
{
	assert(i_alignment);
	assert(IsPowerOfTwo(i_alignment));
	return (((i_memAddr)+(i_alignment - 1)) & ~(i_alignment - 1));
}
