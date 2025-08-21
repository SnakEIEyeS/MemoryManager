#pragma once

//#include "Engine-init.h"
#include "stdint.h"

struct BlockDescriptor;


	
		class HeapManager
		{

		public:
			static HeapManager * create(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
			void destroy();

			void * _alloc(size_t size);
			void * _alloc(size_t size, unsigned int alignment);
			bool _free(void * i_ptr);

			void collect();

			bool Contains(void * i_ptr) const;
			bool IsAllocated(void * i_ptr) const;

			size_t getLargestFreeBlock() const;
			size_t getTotalFreeMemory() const;

			void ShowFreeBlocks() const;
			void ShowOutstandingAllocations() const;

		private:
			HeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);

			void * AllocateFromBlockEnd(BlockDescriptor & i_Block, size_t i_size, unsigned int i_alignment);

			void InitializeFreeDescriptors(BlockDescriptor * i_pFirstDescriptor, size_t i_numDescriptors);

			BlockDescriptor * GetFreeDescriptor();
			void ReturnFreeDescriptor(BlockDescriptor * i_pDesc);
			bool IsValidDescriptorPtr(const BlockDescriptor * i_pDesc) const;

			static unsigned int ListItems(const BlockDescriptor * i_pDesc);
			static void InsertInListMemorySequentially(BlockDescriptor * i_pDesc, BlockDescriptor * & io_pList);
			static bool RemoveFromList(BlockDescriptor * i_pDesc, BlockDescriptor * & io_pList);

			BlockDescriptor * m_pFreeMemory;
			BlockDescriptor * m_pOutstandingMemory;

			BlockDescriptor * m_pFreeDescriptors;

			uint8_t * m_pBase;
			size_t m_sizeBase;

			BlockDescriptor * m_pStartDescriptors;
			unsigned int m_numDescriptors;

			uint8_t * m_pAllocatable;
			size_t m_sizeAllocatable;

			static const unsigned int s_defaultAlignment;
			static const size_t s_minimumToLeave;
		};

		//#include "HeapManagerUtilities.h"
#include "HeapManager-inl.h"
	