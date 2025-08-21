#pragma once

#include "assert.h"
#include <iostream>

#include "Point2D.h"

class Player
{
public:
	//Standard Constructors
	Player(char i_name[]) : name(_strdup(i_name ? i_name : "Unnamed"))
	{
		//name = name;
		//std::cout << "\nRunning Player's standard constructor with 1 parameter.\n";
	}

	Player(char i_name[], Point2D i_pos) : name(_strdup(i_name ? i_name : "Unnamed")), pos(i_pos)
	{
		//name = name;
		//std::cout << "\nRunning Player's standard constructor with 2 parameters.\n";
	}

	//Destructor
	~Player()
	{
	}

	//Copy Constructor
	Player(const Player & i_other) : pos(i_other.pos)
	{
		//std::cout << "\nCreating Player with copy constructor. name = " << i_other.name ? i_other.name : "Unnamed\n";
		name = _strdup(i_other.name ? i_other.name : nullptr);
	}

	//Move Copy Constructor
	Player(Player && i_other) : name(i_other.name), pos(i_other.pos)
	{
		//std::cout << "\nRunning Player's move copy constructor. name = " << i_other.name ? i_other.name : "Unnamed\n";
		i_other.name = nullptr;
	}

	//Assignment Operator
	Player & operator=(const Player & i_other)
	{
		//std::cout << "\nRunning Player's assignment operator.\n";
		//if (name)
			//delete[](name);

		name = _strdup(i_other.name ? i_other.name : "Unnamed");
		pos = i_other.pos;

		return *this;
	}

	//Move Assignment Operator
	Player & operator=(Player && i_other)
	{
		//std::cout << "\nRunning Player's move assignment operator.\n";

		std::swap(name, i_other.name);
		std::swap(pos, i_other.pos);

		return *this;
	}


	void startpos(int startx, int starty);

	void setpos(const Point2D & set);

	int getx() const;

	int gety() const;

private:
	const char * name;
	Point2D pos;
};