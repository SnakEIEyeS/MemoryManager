#include <algorithm>
#include "assert.h"
#include <iostream>
#include <vector>

#include "Allocators.h"
#include "Engine-init.h"
#include "FixedSizeAllocator.h"

//Fixed Size Allocator Unit Test
bool FSA_UnitTest()
{
	std::cout << "\nFixed Size Allocator Unit Test\n\n";

	std::vector<void *> AllocatedAddresses;

	size_t numAllocTries = 0;
	size_t numAllocSuccess = 0;
	size_t numFreeSuccess = 0;

	const size_t toTestNumAllocs = 3000;

	const size_t maxTestAllocationSize = 128;
	const unsigned int freeDeterminer = 5;

	//Try allocating from FSAs until 'toTestNumAllocs' attempts at allocation are done 
	while (numAllocTries < toTestNumAllocs)
	{
		size_t sizeAlloc = 1 + (rand() & (maxTestAllocationSize - 1));

		Engine::FixedSizeAllocator * pFSA = Engine::FindFixedSizeAllocator(sizeAlloc);
		assert(pFSA);

		if (pFSA)
		{
			void * pPtr = pFSA->alloc();

			//If allocation succeeded put the pointer to memory into our vector of addresses
			//Increment number of allocations done
			if (pPtr)
			{
				AllocatedAddresses.push_back(pPtr);

				std::cout << "Allocated Address: " << pPtr << "\tSize: " << sizeAlloc << "\n\n";

				numAllocSuccess++;
			}
			numAllocTries++;
		}

		//Free from the FSAs at random intervals
		if (!AllocatedAddresses.empty() && (rand() % freeDeterminer == 0))
		{
			//Get a random address to free
			std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());
			void * pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			pFSA = Engine::FindFixedSizeAllocatorContains(pPtr);

			if (pFSA)
			{
				bool success = pFSA->free(pPtr);
				assert(success);

				std::cout << "Freed Address: " << pPtr << "\n\n";

				numFreeSuccess++;
			}
		}
	}

	std::cout << "\nAllocation phase completed. Freeing up remaining allocations.\n\n";

	//Free up any remaining allocations from the FSAs
	if (!AllocatedAddresses.empty())
	{
		//Randomize order of adddresses before freeing
		std::random_shuffle(AllocatedAddresses.begin(), AllocatedAddresses.end());

		while (!AllocatedAddresses.empty())
		{
			void* pPtr = AllocatedAddresses.back();
			AllocatedAddresses.pop_back();

			Engine::FixedSizeAllocator * pFSA = Engine::FindFixedSizeAllocatorContains(pPtr);
			assert(pFSA);

			if (pFSA)
			{
				bool success = pFSA->free(pPtr);
				assert(success);

				std::cout << "Freed Address: " << pPtr << "\n\n";

				numFreeSuccess++;
			}
		}	
	}

	//Check if number of allocations and frees are equal, i.e. all allocations have been freed
	assert(numFreeSuccess == numAllocSuccess);

	std::cout << "\nNumber of allocations done: " << numAllocSuccess;
	std::cout << "\nNumber of allocations freed: " << numFreeSuccess;

	std::cout << "\n\nFixed Size Allocator unit test passed!\n\n";

	return true;
}