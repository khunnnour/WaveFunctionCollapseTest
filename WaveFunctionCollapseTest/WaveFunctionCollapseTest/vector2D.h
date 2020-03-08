#pragma once

struct vector2D
{
public:
	// - Members - //
	int x, y;


	// - Constructors - //
	vector2D() { x = 0; y = 0; }
	vector2D(int xDir, int yDir) { x = xDir; y = yDir; }


	//  - Operator overloads - //
	vector2D& operator=(vector2D other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}
	friend vector2D operator+(vector2D l, const vector2D& r)
	{
		vector2D v(l.x + r.x, l.y + r.y);
		return v;
	}
	bool operator==(vector2D r)
	{
		if (x == r.x && y == r.y)
			return true;
		else
			return false;
	}
};