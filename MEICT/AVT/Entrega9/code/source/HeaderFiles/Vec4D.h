#pragma once
#ifndef Vec4D_H
#define Vec4D_H

#include <iostream>
#include <GL/glew.h>
#include <vector>

class Vec4D {
private:
	float v_x;
	float v_y;
	float v_z;
	float v_w;

public:
	Vec4D(void);
	Vec4D(float x, float y, float z, float w);
	void SetVector(float x, float y, float z, float w);
	Vec4D add(Vec4D vec);
	Vec4D sub(Vec4D vec);
	Vec4D mul(float c);
	float dot(Vec4D vec);
	Vec4D operator*(float b);
	friend Vec4D operator*(float a, Vec4D b);
	Vec4D operator+(Vec4D b);
	Vec4D operator-(Vec4D b);
	bool operator==(Vec4D b);
	bool operator!=(Vec4D b);
	float operator[](int i);
    friend std::ostream& operator<<(std::ostream& a, Vec4D b);
	Vec4D normalize();
	float getX();
	float getY();
	float getZ();
	float getW();
	std::vector<GLfloat> toGLfloats();
};
#endif // !Vec4D_H