#pragma once
#ifndef Quat_H
#define Quat_H

#include <iostream>
#include "Vec4D.h"
#include "Matrix4D.h"

class Quat {
private:
	float t, x, y, z;

public:
	Quat(float a, float b, float c, float d);
	void SetQuat(float a, float b, float c, float d);
	
	float getT();
	float getX();
	float getY();
	float getZ();

	static Quat qFromAngleAxis(float theta, Vec4D axis);
	void qToAngleAxis(Quat q, float theta, Vec4D axis);
	Quat normalize();
	Quat mul( float s);
	Quat mul( Quat q1);
	Quat add(Quat q1);
	void rotationMatrix( Matrix4D &m);
	Quat lerp( Quat q1, float k);
	Quat slerp( Quat q1, float k);
};
#endif // !Quat_H