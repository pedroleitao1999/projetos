#pragma once
#ifndef Matrix3D_H
#define Matrix3D_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../HeaderFiles/Vec3D.h"
#include "../HeaderFiles/Matrix4D.h"
using namespace std;

class Matrix3D {
private:
    vector<Vec3D> vectors;

public:
    Matrix3D(vector<Vec3D> v);
    Matrix3D(vector<float> f);

    void SetMatrix(vector<Vec3D> v);
    void SetMatrix(vector<float> f);

    static Matrix3D identity();

	Vec3D getVec(int i);
    Vec3D operator[](int i);

    GLfloat* toGLfloats();

	Matrix3D add(Matrix3D b);
	Matrix3D sub(Matrix3D b);
	Matrix3D operator+(Matrix3D b);
	Matrix3D operator-(Matrix3D b);

	Matrix3D mul(float b);
	Matrix3D mul(Matrix3D b);
	Vec3D mul(Vec3D b);
	Matrix3D operator*(float b);
	Matrix3D operator*(Matrix3D b);
	Vec3D operator*(Vec3D b);
	bool operator==(Matrix3D b);
	bool operator!=(Matrix3D b);

	Matrix3D transpose();
	Matrix3D inverse();
	float det();

	friend std::ostream& operator<<(std::ostream& a, Matrix3D b);
	friend Matrix3D operator*(float a, Matrix3D b);

	Matrix4D convertTo4D();
	Matrix4D convertToView(Vec3D e);
};
#endif // !Matrix3D_H
