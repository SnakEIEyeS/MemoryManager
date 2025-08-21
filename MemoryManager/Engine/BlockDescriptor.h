#pragma once

#include "stdint.h"

struct BlockDescriptor
{
	uint8_t * m_pBase;
	size_t m_size;
	BlockDescriptor * m_pNextDesc;

	uint8_t * m_pUserBlock;
	size_t m_sizeUserBlock;

	BlockDescriptor()
	{}
};
