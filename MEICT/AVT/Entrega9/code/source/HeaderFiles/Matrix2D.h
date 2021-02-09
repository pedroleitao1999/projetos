#pragma once
#ifndef Matrix2D_H
#define Matrix2D_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../HeaderFiles/Vec2D.h"

using namespace std;

class Matrix2D {
private:
	std::vector<Vec2D> vectors;

public:
	Matrix2D(vector<Vec2D> v);
    Matrix2D(vector<float> f);

	void SetMatrix(vector<Vec2D> v);
    void SetMatrix(vector<float> f);

    static Matrix2D identity();

	Vec2D getVec(int i);
    Vec2D operator[](int i);

    GLfloat* toGLfloats();

	Matrix2D add(Matrix2D b);
	Matrix2D sub(Matrix2D b);
	Matrix2D operator+(Matrix2D b);
	Matrix2D operator-(Matrix2D b);

    Matrix2D mul(float b);
	Matrix2D mul(Matrix2D b);
	Vec2D mul(Vec2D b);
	Matrix2D operator*(float b);
	Matrix2D operator*(Matrix2D b);
	Vec2D operator*(Vec2D b);
	bool operator==(Matrix2D b);
	bool operator!=(Matrix2D b);
	friend Matrix2D operator*(float a, Matrix2D b);

	Matrix2D transpose();
	Matrix2D inverse();
	float det();

	friend std::ostream& operator<<(std::ostream& a, Matrix2D b);

};
#endif // !Matrix2D_H
