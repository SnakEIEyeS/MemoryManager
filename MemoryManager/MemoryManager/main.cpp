#include <conio.h>
#include <iostream>

#include "Allocators.h"
#include "Engine-init.h"
#include "HeapManager.h"

extern bool FSA_UnitTest();
extern bool HeapManager_UnitTest();
extern void MonsterChase();

int main()
{
	Engine::EngineInit();

	//Run the Heap Manager Unit Test
	HeapManager_UnitTest();

	std::cout << "Heap Manager Unit Test passed!";

	std::cout << "\n\n\n\n";

	//Reset the heaps after unit test
	Engine::ResetHeap();
	Engine::StartUp();
	
	//Run the Fixed Size Allocator Unit Test
	FSA_UnitTest();

	std::cout << "\n\n\n\n";

	//Run our Monster Chase game
	MonsterChase();

	Engine::ShutDown();

	_getch();
	return 0;
}