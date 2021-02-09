#pragma once
#ifndef Camera_H
#define Camera_H

#include <iostream>
#include "../HeaderFiles/Vec3D.h"
#include "../HeaderFiles/Matrix3D.h"
#include "../HeaderFiles/Matrix4D.h"
#include <vector>


using namespace std;


class Camera {
private:
	Vec3D eye;
	Vec3D center;
	Vec3D up;
	Vec3D s;
	Vec3D v;
	Vec3D u;

public:
	Camera(void);
	Camera(vector<Vec3D> v);
	void SetCamera(vector<Vec3D> v);
	Vec3D getEye();
	Vec3D getCenter();
	Vec3D getUp();
	Vec3D getS();
	Vec3D getV();
	Vec3D getU();
	Vec3D getNegativeV();
	Matrix4D convertToView(Matrix3D m, Vec3D e);
	static Matrix4D perspectiveP(float fov, float aspect, float zNear, float zFar);
	static Matrix4D ortographicP(float left, float right, float bottom, float top, float near, float far);
};
#endif // !Camera_H