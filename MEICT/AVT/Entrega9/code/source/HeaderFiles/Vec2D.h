#pragma once
#ifndef Vec2D_H
#define Vec2D_H

#include <iostream>

class Vec2D {
private:
	float v_x;
	float v_y;

public:
	Vec2D(float x, float y);
	void SetVector(float x, float y);
	Vec2D add(Vec2D vec);
	Vec2D sub(Vec2D vec);
	Vec2D mul(float c);
	Vec2D operator+(Vec2D b);
	Vec2D operator*(float b);
	friend Vec2D operator*(float a, Vec2D b);
	Vec2D operator-(Vec2D b);
	bool operator==(Vec2D b);
	bool operator!=(Vec2D b);
	float operator[](int i);
	float dot(Vec2D vec);
	friend std::ostream& operator<<(std::ostream& a, Vec2D b);
	float getX();
	float getY();

};
#endif // !Vec2D_H