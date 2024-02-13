#include <iostream>


class Shape
{
public:
	int type = 0;
	int a = 0;
	int b = 0;
	int c = 0;

	int getType()
	{
		return type;
	}

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


	Shape(int _type, int _x1, int _y1, int _z1, int _x2, int _y2)
	{
		type = _type;
		x1 = _x1; y1 = _y1;
		x2 = _x2; y2 = _y2;

		// стороны фигуры
		a = abs(x1 - x2);
		b = abs(y1 - y2);
		c = abs(z1 - z2);
	}
	Shape();
	int get_square()
	{
		return 0;
	}
};



class sqr : public Shape
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


class cube : public Shape
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

class circle : public Shape
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



class cylinder : public Shape
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



class transform
{
public:

	transform(const Shape& sh);
	Shape shift(int m, int n, int k);
	Shape scaleX(int a);
	Shape scaleY(int d);
	Shape scaleZ(int e);
	Shape scale(int s);

	static const int line = 0;
	static const int sqr = 1;
	static const int cube = 2;
	static const int circle = 3;
	static const int cylinder = 4;


	transform(Shape& sh)
	{
		type = sh.getType();
		shape = sh;
	}

	Shape shift(int m, int n, int k)
	{
		switch (type)
		{
		case line:
		case circle:
		case cylinder:
			shape.x1 += m; shape.y1 += n;
			shape.x2 += m; shape.y2 += n;
			break;
		case sqr:
			shape.x1 += m; shape.y1 += n;
			shape.x2 += m; shape.y2 += n;
			shape.x3 += m; shape.y3 += n;
			shape.x4 += m; shape.y4 += n;
			break;
		case cube:
			shape.x1 += m; shape.y1 += n; shape.z1 += k;
			shape.x2 += m; shape.y2 += n; shape.z2 += k;
			shape.x3 += m; shape.y3 += n; shape.z3 += k;
			shape.x4 += m; shape.y4 += n; shape.z4 += k;
			shape.x5 += m; shape.y5 += n; shape.z5 += k;
			shape.x6 += m; shape.y6 += n; shape.z6 += k;
			shape.x7 += m; shape.y7 += n; shape.z7 += k;
			shape.x8 += m; shape.y8 += n; shape.z8 += k;
			break;
		}
		return shape;
	}


	Shape scaleX(int a)
	{
		switch (type)
		{
		case line:
		case circle:
		case cylinder:
			shape.x1 *= a;
			shape.x2 *= a;
			break;
		case sqr:
			shape.x1 *= a;
			shape.x2 *= a;
			shape.x3 *= a;
			shape.x4 *= a;
			break;
		case cube:
			shape.x1 *= a;
			shape.x2 *= a;
			shape.x3 *= a;
			shape.x4 *= a;
			shape.x5 *= a;
			shape.x6 *= a;
			shape.x7 *= a;
			shape.x8 *= a;
			break;
		}
		return shape;
	}

	Shape scaleY(int d)
	{
		switch (type)
		{
		case line:
		case circle:
		case cylinder:
			shape.y1 *= d;
			shape.y2 *= d;
			break;
		case sqr:
			shape.y1 *= d;
			shape.y2 *= d;
			shape.y3 *= d;
			shape.y4 *= d;
			break;
		case cube:
			shape.y1 *= d;
			shape.y2 *= d;
			shape.y3 *= d;
			shape.y4 *= d;
			shape.y5 *= d;
			shape.y6 *= d;
			shape.y7 *= d;
			shape.y8 *= d;
			break;
		}
		return shape;
	}

	Shape scaleZ(int e)
	{
		switch (type)
		{
		case line:
		case circle:
		case cylinder:
			shape.z1 *= e;
			shape.z2 *= e;
			break;
		case sqr:
			shape.z1 *= e;
			shape.z2 *= e;
			shape.z3 *= e;
			shape.z4 *= e;
			break;
		case cube:
			shape.z1 *= e;
			shape.y2 *= e;
			shape.z3 *= e;
			shape.z4 *= e;
			shape.z5 *= e;
			shape.z6 *= e;
			shape.z7 *= e;
			shape.z8 *= e;
			break;
		}
		return shape;
	}

	Shape scale(int s)
	{
		switch (type)
		{
		case line:
		case circle:
		case cylinder:
			shape.x1 /= s; shape.y1 /= s;
			shape.x2 /= s; shape.y2 /= s;
			break;
		case sqr:
			shape.x1 /= s; shape.y1 /= s;
			shape.x2 /= s; shape.y2 /= s;
			shape.x3 /= s; shape.y3 /= s;
			shape.x4 /= s; shape.y4 /= s;
			break;
		case cube:
			shape.x1 /= s; shape.y1 /= s; shape.z1 /= s;
			shape.x2 /= s; shape.y2 /= s; shape.z2 /= s;
			shape.x3 /= s; shape.y3 /= s; shape.z3 /= s;
			shape.x4 /= s; shape.y4 /= s; shape.z4 /= s;
			shape.x5 /= s; shape.y5 /= s; shape.z5 /= s;
			shape.x6 /= s; shape.y6 /= s; shape.z6 /= s;
			shape.x7 /= s; shape.y7 /= s; shape.z7 /= s;
			shape.x8 /= s; shape.y8 /= s; shape.z8 /= s;
			break;
		}

		return shape;
	}

private:
	Shape shape;
	int type = 0;
};



int main()
{

}
