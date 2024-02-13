#pragma once
#pragma once
#include"shape.h"
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

private:
	Shape shape;
	int type = 0;
};
