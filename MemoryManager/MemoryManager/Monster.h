#pragma once

#include "assert.h"
#include <iostream>

#include "Point2D.h"


class Monster
{
public:
	//static Monster * createMonster(char * i_name, int i_mnum, int i_setlife, Point2D i_pos);

	//Standard Constructors
	Monster()
	{}

	Monster(int mnum, int setlife)
	{
		assert(setlife > 0);
		mnum = mnum;
		name = "Unnamed";
		life = setlife;
		std::cout << name << " created.\n";
	}

	Monster(char * i_name, int i_mnum, int i_setlife, Point2D i_pos) : name(_strdup(i_name ? i_name : "Unnamed")), mnum(i_mnum), life(i_setlife), pos(i_pos)
	{
		//std::cout << "\nRunning Monster's constructor with 4 parameters.\n";
	}

	//Destructor
	~Monster()
	{
	}

	//Copy Constructor
	Monster(const Monster & i_other) : mnum(i_other.mnum), life(i_other.life), pos(i_other.pos)
	{
		//std::cout << "\nRunning Monster's copy constructor. name = " << i_other.name ? i_other.name : "Unnamed\n";
		name = _strdup(i_other.name ? i_other.name : "Unnamed");
	}

	//Move Copy Constructor
	Monster(Monster && i_other) : name(i_other.name), mnum(i_other.mnum), life(i_other.life), pos(i_other.pos)
	{
		//std::cout << "\nRunning Monster's move copy constructor. name = " << i_other.name ? i_other.name : "Unnamed\n";
		i_other.name = nullptr;
	}

	//Assignment Operator
	Monster & operator=(const Monster & i_other)
	{
		//std::cout << "\nRunning Monster's assignment operator.\n";

		name = _strdup(i_other.name ? i_other.name : "Unnamed");
		mnum = i_other.mnum;
		life = i_other.life;
		pos = i_other.pos;

		return *this;
	}

	//Move Assignment Operator
	Monster & operator=(Monster && i_other)
	{
		//std::cout << "\nRunning Monster's move assignment operator.\n";

		std::swap(name, i_other.name);
		std::swap(mnum, i_other.mnum);
		std::swap(life, i_other.life);
		std::swap(pos, i_other.pos);

		return *this;
	}

	int getx() const;

	int gety() const;

	int getlife() const;

	void startpos(int startx, int starty);

	void getname();

	int getnum() const;

	void writepos();

	void setpos(const Point2D & set);

	void bounceBack(const Point2D & bDirection);

	void onelife();

private:
	char * name;
	int mnum, life;
	Point2D pos;
};