//#include "GameObjectController.h"
#include "Monster.h"

#include "assert.h"
#include <iostream>

inline int Monster::getx() const
{
	return pos.getx();
}

inline int Monster::gety() const
{
	return pos.gety();
}

inline int Monster::getlife() const
{
	return life;
}

inline void Monster::startpos(int startx, int starty)
{
	assert(128 >= startx || startx >= 0);
	assert(128 >= starty || starty >= 0);
	pos.setx(startx);
	pos.sety(starty);
}

inline void Monster::getname()
{
	std::cout << name;
}

inline int Monster::getnum() const
{
	return mnum;
}

inline void Monster::writepos()
{
	std::cout << "Monster " << name << " is at " << pos.getx() << ", " << pos.gety() << ".\n";
}

inline void Monster::setpos(const Point2D & set)
{
	pos = pos + set;
}

inline void Monster::bounceBack(const Point2D & bDirection)
{
	pos = pos + bDirection;
}

inline void Monster::onelife()
{
	life--;
}


/*class MonsterController : public GameObjectController
{
public:
void setGameObject(GameObject * pGameObj) override
{
mGameObject = pGameObj;
}

void setFocusObject(GameObject * i_pGameObj)
{
mFocus = i_pGameObj;
}

void updateGameObject() override
{
Point2D wanderPos;
if (mFocus)
{
wanderPos = normalizeWPos();
mGameObject->setPos(mGameObject->getPos() + wanderPos);
}
else
{
wanderPos = getWanderDir();
mGameObject->setPos(mGameObject->getPos() + wanderPos);
}

if (mGameObject->getPos().getx() > 128)
mGameObject->setPos(mGameObject->getPos() - Point2D(3, 0));
else if (mGameObject->getPos().getx() < 0)
mGameObject->setPos(mGameObject->getPos() + Point2D(3, 0));

if (mGameObject->getPos().gety() > 128)
mGameObject->setPos(mGameObject->getPos() - Point2D(0, 3));
else if (mGameObject->getPos().gety() < 0)
mGameObject->setPos(mGameObject->getPos() + Point2D(0, 3));
}

Point2D normalizeWPos()
{
Point2D retPoint = mFocus->getPos() - mGameObject->getPos();

if(retPoint.getx() > 0)
retPoint.setx(1);
else if(retPoint.getx() < 0)
retPoint.setx(-1);
else if(retPoint.getx() == 0)
retPoint.setx(0);

if(retPoint.gety() > 0)
retPoint.sety(1);
else if (retPoint.gety() < 0)
retPoint.sety(-1);
else if (retPoint.gety() == 0)
retPoint.sety(0);

return retPoint;
}

Point2D getWanderDir()
{
uint8_t rMonMove = rand() % 8;
switch (rMonMove)
{
case 0: return Point2D(1, 0);
break;
case 1: return Point2D(1, -1);
break;
case 2: return Point2D(0, -1);
break;
case 3: return Point2D(-1, -1);
break;
case 4: return Point2D(-1, 0);
break;
case 5: return Point2D(-1, 1);
break;
case 6: return Point2D(0, 1);
break;
case 7: return Point2D(1, 1);
break;
default: return Point2D(0, 0);
break;
}
}

private:
GameObject * mGameObject;
GameObject * mFocus;
};*/

/*Monster * Monster::createMonster(char * i_name, int i_mnum, int i_setlife, Point2D i_pos)
{
assert(i_name);
assert(i_mnum);
assert(i_setlife);

return new Monster(i_name, i_mnum, i_setlife, i_pos);
}*/