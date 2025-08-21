#include "assert.h"
#include "conio.h"
#include <iostream>
#include <time.h>
#include <vector>

#include "Allocators.h"
#include "Engine-init.h"
#include "Monster.cpp"
#include "Player.cpp"
#include "Point2D.h"

//#include "PodBotController.h"
//extern bool Point2DunitTest();

//The Monster Chase Game
void MonsterChase()
{
	WINDOWS();
	DEBUGPRINT();
	EMIT_LOC();


	//PodBotController * podBot = new PodBotController(new GameObject(Point2D(30, 30), "PodBot"));


	//Point2DunitTest();

	//Declarations for the game
	char c;
	bool end = false;

	Point2D up = Point2D(0, -1);
	Point2D down = Point2D(0, 1);
	Point2D left = Point2D(-1, 0);
	Point2D right = Point2D(1, 0);
	Point2D upRight = Point2D(1, -1);
	Point2D upLeft = Point2D(-1, -1);
	Point2D downLeft = Point2D(-1, 1);
	Point2D downRight = Point2D(1, 1);
	Point2D bounce = Point2D(3, 3);
	
	char pname[50];

	std::vector<Monster> monsterVec;
	unsigned int mnum = 0;
	unsigned int maxmon = 0, creamon = 0;
	//Monster * monsters2;

	srand((unsigned)time(NULL));

	//Game starts here
	//Initialize Player
	std::cout << "Hi! Welcome to Monster Chase!\nEnter your name:";
	std::cin.get(pname, 50, '\n');
	//assert(pname[0] != '\0' && "Player name cannot be empty.");
	Player player = Player(pname);
	player.startpos(rand() % 128, rand() % 128);

	std::cout << "Enter number of monsters:";
	std::cin >> maxmon;
	assert(maxmon > 0);
	unsigned int initmon = maxmon;

	std::cout << "There will always be a minimum of " << initmon << " monsters in the game.\nGet away from them! Move using the W, S, A & D keys. Press Q or ESC to quit.\n\n";

	std::cout << "\nPlayer Name: " << pname << "\n\n";
	assert(128 >= player.getx() || player.getx() >= 0);
	assert(128 >= player.gety() || player.gety() >= 0);
	std::cout << "The grid is 128*128. You are at " << player.getx() << ", " << player.gety() << ".\n";

	//Create initial group of Monsters
	//Monster * monsters = (Monster*) malloc(maxmon*sizeof(Monster));
	char * monsterNames[] = { "Rosy", "Pinky", "Grimy", "Shimy", "Walda", "Walden", "Monstah!", "Snoke", "Kylo Ren", "Cookie" };
	for (unsigned int i = 0; i < maxmon; i++)
	{
		monsterVec.push_back(Monster(monsterNames[rand() % (sizeof(monsterNames)/sizeof(monsterNames[0]))], mnum, rand() % 10 + 15, Point2D(rand() % 128, rand() % 128)));
		//monsterVec[i].startpos(rand() % 128, rand() % 128);
		monsterVec[i].writepos();
		mnum++;
	}

	std::cout << "\n";

	EMIT_LOC();

	//Core Game Loop
	while (!end)
	{
		if (_kbhit())
		{
			std::cout << "\n\n----------------------------------------------------\n\n";
			c = _getch();
			switch (c)
			{
				//Directional movement input
			case 'W':
			case 'w': std::cout << "Pressed w to move up.\n";
				player.setpos(up);
				break;

			case 'S':
			case 's': std::cout << "Pressed s to move down.\n";
				player.setpos(down);
				break;

			case 'A':
			case 'a': std::cout << "Pressed a to move left.\n";
				player.setpos(left);
				break;

			case 'D':
			case 'd': std::cout << "Pressed d to move right.\n";
				player.setpos(right);
				break;

				//Quit condition
			case 27:
			case 'Q':
			case 'q': std::cout << "Exiting game...\n";
				end = true;
				break;

			default: std::cout << "Invalid input.\n";
				break;
			}

			if (!end)
			{
				assert(128 >= player.getx() || player.getx() >= 0);
				assert(128 >= player.gety() || player.gety() >= 0);
				std::cout << "You are at " << player.getx() << ", " << player.gety() << ".\n";

				//Move Monsters
				for (unsigned int i = 0; i < maxmon; i++)
				{
					unsigned int rMonMove = rand() % 8;
					switch (rMonMove)
					{
					case 0: monsterVec[i].setpos(right);
						break;
					case 1: monsterVec[i].setpos(upRight);
						break;
					case 2: monsterVec[i].setpos(up);
						break;
					case 3: monsterVec[i].setpos(upLeft);
						break;
					case 4: monsterVec[i].setpos(left);
						break;
					case 5: monsterVec[i].setpos(downLeft);
						break;
					case 6: monsterVec[i].setpos(down);
						break;
					case 7: monsterVec[i].setpos(downRight);
						break;
					default:
						break;
					}

					if (monsterVec[i].getx() > 128)
					{
						monsterVec[i].bounceBack(left * bounce);
					}
					else if (monsterVec[i].getx() < 0)
					{
						monsterVec[i].bounceBack(right * bounce);
					}

					if (monsterVec[i].gety() > 128)
					{
						monsterVec[i].bounceBack(up * bounce);
					}
					else if (monsterVec[i].gety() < 0)
					{
						monsterVec[i].bounceBack(down * bounce);
					}

					monsterVec[i].onelife();
					monsterVec[i].writepos();
				}

				//Check Monsters' life
				for (unsigned int i = 0;;)
				{
					//assert(monsters[i].getlife() >= 0);
					if (monsterVec[i].getlife() == 0 && end != true)
					{
						std::cout << "\nMonster ";
						monsterVec[i].getname();
						std::cout << " died.\n";

						//monsterVec.erase(monsterVec.begin()+i);
						monsterVec[i] = monsterVec.back();
						monsterVec.pop_back();

						
						maxmon--;
						i = 0;
					}
					else
						i++;

					if (i == maxmon)
						break;
				}

				std::cout << "\n";
			}

			creamon++;

			//Create a Monster at appropriate intervals
			if ((creamon % 3 == 0 || maxmon < initmon) && end != true)
			{
				std::cout << "A new monster has spawned. ";

				monsterVec.push_back(Monster(monsterNames[rand() % 7], mnum, rand() % 10 + 15, Point2D(rand() % 128, rand() % 128)));
				monsterVec.back().startpos(rand() % 128, rand() % 128);
				monsterVec.back().writepos();

				std::cout << "\n";
				mnum++;
				maxmon++;
			}

			//podBot->updateGameObject();

			std::cout << "\n\n----------------------------------------------------\n";

		}
	}

	//free (monsters);
	monsterVec.clear();

	_getch();
	return;
}