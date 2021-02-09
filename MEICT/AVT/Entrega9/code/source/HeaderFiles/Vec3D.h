#pragma once
#ifndef Vec3D_H
#define Vec3D_H

#include <iostream>

class Vec3D {
private:
	float v_x;
	float v_y;
	float v_z;

public:
	Vec3D(float x, float y, float z);
	Vec3D();
	void SetVector(float x, float y, float z);
	Vec3D add(Vec3D vec);
	Vec3D sub(Vec3D vec);
	Vec3D mul(float c);
	Vec3D normalize();
	double convertToRadians(float degree);
	Vec3D rotationFormula(float degree, Vec3D k);
	float dot(Vec3D vec);
	Vec3D operator+(Vec3D b);
	bool operator==(Vec3D b);
	bool operator!=(Vec3D b);
	Vec3D operator*(float b);
	float operator[](int i);
	friend Vec3D operator*(float a, Vec3D b);
	Vec3D operator-(Vec3D b);
	friend std::ostream& operator<<(std::ostream& a, Vec3D b);
	Vec3D cross(Vec3D vec);
	float getX();
	float getY();
	float getZ();
};
#endif // !Vec3D_H
