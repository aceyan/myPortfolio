#pragma once

struct Point2D
{
public:
	Point2D():x(0), y(0){};
	Point2D(float x, float y) :x(x), y(y){};
	float x;
	float y;
};