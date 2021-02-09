#pragma once
#ifndef Matrix4D_H
#define Matrix4D_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../HeaderFiles/Vec4D.h"

using namespace std;

class Matrix4D {
private:
	std::vector<Vec4D> vectors;

public:
	Matrix4D(vector<Vec4D> v);
	Matrix4D(vector<float> f);
	Matrix4D();

	void SetMatrix(vector<Vec4D> v);
	void SetMatrix(vector<float> f);

	static Matrix4D identity();
	static Matrix4D scale(float sx, float sy, float sz);
    static Matrix4D translate(float dx, float dy, float dz);
    static Matrix4D rotate(float angle, char s);

	Vec4D getVec(int n);
    Vec4D operator[](int i);

    GLfloat* toGLfloats();

	Matrix4D add(Matrix4D b);
	Matrix4D sub(Matrix4D b);
	Matrix4D operator+(Matrix4D b);
	Matrix4D operator-(Matrix4D b);


	Matrix4D mul(float b);
	Matrix4D mul(Matrix4D b);
	Vec4D mul(Vec4D b);
	Matrix4D operator*(float b);
	Matrix4D operator*(Matrix4D b);
	Vec4D operator*(Vec4D b);
	bool operator==(Matrix4D b);
	bool operator!=(Matrix4D b);
	friend Matrix4D operator*(float a, Matrix4D b);

	Matrix4D transpose();
	friend std::ostream& operator<<(std::ostream& a, Matrix4D b);
	
};
#endif // !Matrix4D_H
