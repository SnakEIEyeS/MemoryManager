#include "Player.h"

#include "assert.h"
#include "conio.h"
#include <iostream>

//#include "GameObjectController.h"


inline void Player::startpos(int startx, int starty)
{
	assert(128 >= startx || startx >= 0);
	assert(128 >= starty || starty >= 0);
	pos = Point2D(startx, starty);
}

inline void Player::setpos(const Point2D & set)
{
	int currx = pos.getx();
	//assert(128 >= currx || currx >= 0);
	int curry = pos.gety();
	//assert(128 >= curry || curry >= 0);
	if (128 >= currx + set.getx() && currx + set.getx() >= 0 && 128 >= curry + set.gety() && curry + set.gety() >= 0)
		pos = pos + set;
	else
		std::cout << "You're at the edge of the grid! Can't go any further this way! Change directions, NOW!\n";
}

inline int Player::getx() const
{
	return pos.getx();
}

inline int Player::gety() const
{
	return pos.gety();
}

/*class PlayerController : public GameObjectController
{
void setGameObject(GameObject * pGameObj) override
{
mGameObject = pGameObj;
}

void updateGameObject() override
{
Point2D iDirection = getMovementDirectionFromUser();

if(128 >= mGameObject->getPos().getx() + iDirection.getx() && mGameObject->getPos().getx() + iDirection.getx() >= 0
&& 128 >= mGameObject->getPos().gety() + iDirection.gety() && mGameObject->getPos().gety() + iDirection.gety() >= 0)
{ mGameObject->setPos(mGameObject->getPos() + iDirection); }
else
{ std::cout << "You're at the edge of the grid! Can't go any further this way! Change directions, NOW!\n"; }
}

Point2D getMovementDirectionFromUser()
{
char ip = _getch();
switch (ip)
{
//Directional movement input
case 'W':
case 'w': std::cout << "Pressed w to move up.\n";
return Point2D(0, -1);
break;

case 'S':
case 's': std::cout << "Pressed s to move down.\n";
return Point2D(0, 1);
break;

case 'A':
case 'a': std::cout << "Pressed a to move left.\n";
return Point2D(-1, 0);
break;

case 'D':
case 'd': std::cout << "Pressed d to move right.\n";
return Point2D(1, 0);
break;

default: std::cout << "Invalid input.\n";
return Point2D(0, 0);
break;
}
}

private:
GameObject * mGameObject;
};*/
