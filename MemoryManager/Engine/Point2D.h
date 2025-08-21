#pragma once

#include "assert.h"

class Point2D
{
public:
	inline Point2D()
	{}

	inline Point2D(int i_x, int i_y)
	{
		assert(128 >= i_x || i_x >= 0);
		assert(128 >= i_y || i_y >= 0);
		m_x = i_x;
		m_y = i_y;
	}

	int getx() const
	{
		return m_x;
	}

	int gety() const
	{
		return m_y;
	}

	void setx(int i_x)
	{
		assert(128 >= i_x || i_x >= 0);
		m_x = i_x;
	}

	void sety(int i_y)
	{
		assert(128 >= i_y || i_y >= 0);
		m_y = i_y;
	}

private:
	int m_x;
	int m_y;
};


inline Point2D operator+(const Point2D & i_lhs, const Point2D & i_rhs)
{
	Point2D fi_lhs = i_lhs;
	Point2D fi_rhs = i_rhs;
	return Point2D(fi_lhs.getx() + fi_rhs.getx(), fi_lhs.gety() + fi_rhs.gety());
}

inline Point2D operator-(const Point2D & i_lhs, const Point2D & i_rhs)
{
	Point2D fi_lhs = i_lhs;
	Point2D fi_rhs = i_rhs;
	return Point2D(fi_lhs.getx() - fi_rhs.getx(), fi_lhs.gety() - fi_rhs.gety());
}

inline Point2D operator*(const Point2D & i_lhs, const Point2D & i_rhs)
{
	Point2D fi_lhs = i_lhs;
	Point2D fi_rhs = i_rhs;
	return Point2D(fi_lhs.getx() * fi_rhs.getx(), fi_lhs.gety() * fi_rhs.gety());
}

inline void operator+=(const Point2D & i_lhs, const Point2D & i_rhs)
{
	Point2D fi_lhs = i_lhs;
	Point2D fi_rhs = i_rhs;
	fi_lhs.setx(fi_lhs.getx() + fi_rhs.getx());
	fi_lhs.sety(fi_lhs.gety() + fi_rhs.gety());
}

inline void operator-=(const Point2D & i_lhs, const Point2D & i_rhs)
{
	Point2D fi_lhs = i_lhs;
	Point2D fi_rhs = i_rhs;
	fi_lhs.setx(fi_lhs.getx() - fi_rhs.getx());
	fi_lhs.sety(fi_lhs.gety() - fi_rhs.gety());
}