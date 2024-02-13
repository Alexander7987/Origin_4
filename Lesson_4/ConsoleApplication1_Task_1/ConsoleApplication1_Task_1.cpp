#include <iostream>

class line
{
public:
	int type = 0;
	int a = 0;
	int b = 0;
	int c = 0;

	int getType();

	int x1 = 0, y1 = 0, z1 = 0,
		x2 = 0, y2 = 0, z2 = 0,
		x3 = 0, y3 = 0, z3 = 0,
		x4 = 0, y4 = 0, z4 = 0,
		x5 = 0, y5 = 0, z5 = 0,
		x6 = 0, y6 = 0, z6 = 0,
		x7 = 0, y7 = 0, z7 = 0,
		x8 = 0, y8 = 0, z8 = 0;

	double volume;
	double square;
	double height;
	double radius;


	line(int _type, int _x1, int _y1, int _z1, int _x2, int _y2)
	{
		type = _type;
		x1 = _x1; y1 = _y1;
		x2 = _x2; y2 = _y2;

		// стороны фигуры
		a = abs(x1 - x2);
		b = abs(y1 - y2);
		c = abs(z1 - z2);
	}
	line();
	int get_square()
	{
		return 0;
	}
};



class sqr : public line
{
	sqr(int _type, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, int _x4, int _y4, int _x5, int _y5, int _x6, int _y6, int _x7, int _y7, int _x8, int _y8)
	{
		type = _type;
		x1 = _x1; y1 = _y1;
		x2 = _x2; y2 = _y2;
		x3 = _x3; y3 = _y3;
		x4 = _x4; y4 = _y4;

		// стороны фигуры
		a = abs(x1 - x2);
		b = abs(y1 - y2);
	}

	int get_square()
	{
		square = a * b;
		return square;
	}
};


class cube : public line
{
	cube(int _type, int _x1, int _y1, int _z1, int _x2, int _y2, int _z2, int _x3, int _y3, int _z3, int _x4, int _y4, int _z4, int _x5, int _y5, int _z5, int _x6, int _y6, int _z6, int _x7, int _y7, int _z7, int _x8, int _y8, int _z8)
	{
		type = _type;
		x1 = _x1; y1 = _y1; z1 = _z1;
		x2 = _x2; y2 = _y2; z2 = _z2;
		x3 = _x3; y3 = _y3; z3 = _z3;
		x4 = _x4; y4 = _y4; z4 = _z4;
		x5 = _x5; y5 = _y5; z5 = _z5;
		x6 = _x6; y6 = _y6; z6 = _z6;
		x7 = _x7; y7 = _y7; z7 = _z7;
		x8 = _x8; y8 = _y8; z8 = _z8;

		// стороны фигуры
		a = abs(x1 - x2);
		b = abs(y1 - y2);
		c = abs(z1 - z2);
	}

	int get_square()
	{
		square = 2 * a * b + 2 * a * c + 2 * b * c;
		return square;
	}
};

class circle : public line
{
	circle(int _type, int _x1, int _y1, int R)
	{
		type = _type;
		x1 = _x1; y1 = _y1;
		radius = R;
	}

	int get_square()
	{
		square = 3.14 * radius * radius;
		return square;
	}

};



class cylinder : public line
{
	cylinder(int _type, int _x1, int _y1, int R, int H)
	{
		x1 = _x1; y1 = _y1;
		radius = R;
		height = H;
	}

	int get_square()
	{
		square = 3.14 * radius * radius + 2 * radius * height;
		return square;
	}
};





int main()
{

}
