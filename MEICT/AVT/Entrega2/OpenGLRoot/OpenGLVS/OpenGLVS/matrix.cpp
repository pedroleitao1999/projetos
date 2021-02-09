#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <math.h> 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "matrix.h"
#include "vector.h"

#define PI 3.14159265
using namespace std;

/////////////////////// MATRIX 2x2 //////////////////////////////

Matrix2x2::Matrix2x2(double a, double b, double c, double d)
{
	ux = a;
	uy = b;
	vx = c;
	vy = d;
};

Matrix2x2 Matrix2x2::operator+(const Matrix2x2& a) {
	Matrix2x2 mat = Matrix2x2(0, 0, 0, 0);
	mat.ux = this->ux + a.ux;
	mat.uy = this->uy + a.uy;
	mat.vx = this->vx + a.vx;
	mat.vy = this->vy + a.vy;
	return mat;
};

Matrix2x2 Matrix2x2::operator-(const Matrix2x2& a) {
	Matrix2x2 mat = Matrix2x2(0, 0, 0, 0);
	mat.ux = this->ux - a.ux;
	mat.uy = this->uy - a.uy;
	mat.vx = this->vx - a.vx;
	mat.vy = this->vy - a.vy;
	return mat;
};

Matrix2x2 Matrix2x2::operator*(const Matrix2x2& a) {
	Matrix2x2 mat = Matrix2x2(0, 0, 0, 0);

	Vector2d r1 = Vector2d(this->ux, this->uy);
	Vector2d r2 = Vector2d(this->vx, this->vy);
	Vector2d c1 = Vector2d(a.ux, a.vx);
	Vector2d c2 = Vector2d(a.uy, a.vy);

	mat.ux = Vector2d::dotProduct(r1, c1);
	mat.uy = Vector2d::dotProduct(r1, c2);
	mat.vx = Vector2d::dotProduct(r2, c1);
	mat.vy = Vector2d::dotProduct(r2, c2);

	return mat;
};

Matrix2x2 Matrix2x2::operator*(const double k) {
	Matrix2x2 mat = Matrix2x2(0, 0, 0, 0);
	mat.ux = this->ux * k;
	mat.uy = this->uy * k;
	mat.vx = this->vx * k;
	mat.vy = this->vy * k;
	return mat;
};

Vector2d Matrix2x2::operator*(const Vector2d& vec) {
	Vector2d v = vec;
	double a = v.getX() * this->ux + v.getY() * this->uy;
	double b = v.getX() * this->vx + v.getY() * this->vy;
	Vector2d res = Vector2d(a, b);
	return res;
};

Matrix2x2 Matrix2x2::transpose(void) {
	Matrix2x2 mat = Matrix2x2(0, 0, 0, 0);
	mat.ux = this->ux;
	mat.uy = this->vx;
	mat.vx = this->uy;
	mat.vy = this->vy;
	return mat;
};

double Matrix2x2::determinant(void) {
	return (this->ux * this->vy) - (this->uy * this->vx);
};


Matrix2x2 Matrix2x2::inverse(void) {
	double det = Matrix2x2::determinant();

	if (det == 0) {
		throw "Determinant is zero! Division by zero not possible.";
	}

	Matrix2x2 mat = Matrix2x2(0, 0, 0, 0);
	mat.ux = this->vy;
	mat.uy = -this->uy;
	mat.vx = -this->vx;
	mat.vy = this->ux;

	Matrix2x2 matRes = mat * (1 / det);

	return matRes;
};

/////////////////////// MATRIX 3x3 //////////////////////////////

Matrix3x3::Matrix3x3(double a, double b, double c, double d, double e, double f, double g, double h, double i)
{
	ux = a; uy = b; uz = c;
	vx = d; vy = e; vz = f;
	wx = g; wy = h; wz = i;
};


Matrix3x3 Matrix3x3::operator+(const Matrix3x3& a) {
	Matrix3x3 mat = Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0);
	mat.ux = this->ux + a.ux;
	mat.uy = this->uy + a.uy;
	mat.uz = this->uz + a.uz;

	mat.vx = this->vx + a.vx;
	mat.vy = this->vy + a.vy;
	mat.vz = this->vz + a.vz;

	mat.wx = this->wx + a.wx;
	mat.wy = this->wy + a.wy;
	mat.wz = this->wz + a.wz;

	return mat;
};

Matrix3x3 Matrix3x3::operator-(const Matrix3x3& a) {
	Matrix3x3 mat = Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0);
	mat.ux = this->ux - a.ux;
	mat.uy = this->uy - a.uy;
	mat.uz = this->uz - a.uz;

	mat.vx = this->vx - a.vx;
	mat.vy = this->vy - a.vy;
	mat.vz = this->vz - a.vz;

	mat.wx = this->wx - a.wx;
	mat.wy = this->wy - a.wy;
	mat.wz = this->wz - a.wz;

	return mat;
};

Matrix3x3 Matrix3x3::operator*(const Matrix3x3& a) {
	Matrix3x3 mat = Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0);

	Vector3d r1 = Vector3d(this->ux, this->uy, this->uz);
	Vector3d r2 = Vector3d(this->vx, this->vy, this->vz);
	Vector3d r3 = Vector3d(this->wx, this->wy, this->wz);
	Vector3d c1 = Vector3d(a.ux, a.vx, a.wx);
	Vector3d c2 = Vector3d(a.uy, a.vy, a.wy);
	Vector3d c3 = Vector3d(a.uz, a.vz, a.wz);

	mat.ux = Vector3d::dotProduct(r1, c1);
	mat.uy = Vector3d::dotProduct(r1, c2);
	mat.uz = Vector3d::dotProduct(r1, c3);

	mat.vx = Vector3d::dotProduct(r2, c1);
	mat.vy = Vector3d::dotProduct(r2, c2);
	mat.vz = Vector3d::dotProduct(r2, c3);

	mat.wx = Vector3d::dotProduct(r3, c1);
	mat.wy = Vector3d::dotProduct(r3, c2);
	mat.wz = Vector3d::dotProduct(r3, c3);

	return mat;
};

Matrix3x3 Matrix3x3::operator*(const double k) {
	Matrix3x3 mat = Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0);
	mat.ux = this->ux * k;
	mat.uy = this->uy * k;
	mat.uz = this->uz * k;

	mat.vx = this->vx * k;
	mat.vy = this->vy * k;
	mat.vz = this->vz * k;

	mat.wx = this->wx * k;
	mat.wy = this->wy * k;
	mat.wz = this->wz * k;

	return mat;
};

Vector3d Matrix3x3::operator*(const Vector3d& vec) {
	Vector3d v = vec;
	double a = v.getX() * this->ux + v.getY() * this->uy + v.getZ() * this->uz;
	double b = v.getX() * this->vx + v.getY() * this->vy + v.getZ() * this->vz;
	double c = v.getX() * this->wx + v.getY() * this->wy + v.getZ() * this->wz;
	Vector3d res = Vector3d(a, b, c);
	return res;
};

Matrix3x3 Matrix3x3::transpose(void) {
	Matrix3x3 mat = Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0);
	mat.ux = this->ux;
	mat.uy = this->vx;
	mat.uz = this->wx;

	mat.vx = this->uy;
	mat.vy = this->vy;
	mat.vz = this->wy;

	mat.ux = this->ux;
	mat.uy = this->vx;
	mat.uz = this->wx;

	return mat;
};

double Matrix3x3::determinant(void) {
	double res1 = this->ux * (this->vy * this->wz - this->vz * this->wy);
	double res2 = this->uy * (this->vx * this->wz - this->vz * this->wx);
	double res3 = this->uz * (this->vx * this->wy - this->vy * this->wx);
	return abs(res1 - res2 + res3);
};


Matrix3x3 Matrix3x3::inverse(void) {
	double det = Matrix3x3::determinant();

	if (det == 0) {
		cout << "Determinant is zero! Division by zero not possible.\n" << endl;
	}

	Matrix3x3 mat = Matrix3x3(0, 0, 0, 0, 0, 0, 0, 0, 0);

	mat.ux = this->vy * this->wz - this->vz * this->wy;
	mat.uy = -(this->vx * this->wz - this->vz * this->wx);
	mat.uz = this->vx * this->wy - this->vy * this->wx;

	mat.vx = -(this->uy * this->wz - this->uz * this->wy);
	mat.vy = this->ux * this->wz - this->uz * this->wx;
	mat.vz = -(this->ux * this->wy - this->vy * this->wx);

	mat.wx = this->uy * this->vz - this->uz * this->vy;
	mat.wy = -(this->vx * this->vz - this->uz * this->vx);
	mat.wz = this->vx * this->vy - this->uy * this->vx;

	Matrix3x3 matT = mat.transpose();

	Matrix3x3 matRes = matT * (1 / det);

	return matRes;
};



Vector3d Matrix3x3::RRF(Vector3d v, Vector3d a, double angle) {
	a = a.normalize();
	double num = Vector3d::dotProduct(a, v);
	return v + (a * v) * sin(angle * PI / 180) + ((a * num) - v) * (1 - cos(angle * PI / 180));
};

std::string Matrix3x3::toString() {
	return "ux = " + std::to_string(this->ux) + "\tuy = " + std::to_string(this->uy) + "\tuz = " + std::to_string(this->uz) +
		"\nvx = " + std::to_string(this->vx) + "\tvy = " + std::to_string(this->vy) + "\tvz = " + std::to_string(this->vz) +
		"\nwx = " + std::to_string(this->wx) + "\twy = " + std::to_string(this->wy) + "\twz = " + std::to_string(this->wz);
};

Matrix4x4::Matrix4x4(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n, double o, double p)
{
	ux = a; uy = b; uz = c; un = d;
	vx = e; vy = f; vz = g; vn = h;
	wx = i; wy = j; wz = k; wn = l;
	mx = m; my = n; mz = o; mn = p;
};

Matrix4x4 Matrix4x4::translation(double a, double b, double c) {
	return Matrix4x4(1, 0, 0, a, 0, 1, 0, b, 0, 0, 1, 3, 0, 0, 0, 1);
};

Matrix4x4 Matrix4x4::scale(double a, double b, double c) {
	return Matrix4x4(a, 0, 0, 0, 
					0, b, 0, 0, 
					0, 0, c, 3, 
					0, 0, 0, 1);
};

Matrix4x4 Matrix4x4::rotationY(double angle) {
	return Matrix4x4(cos(angle * PI / 180), 0, sin(angle * PI / 180), 0, 
						0, 1, 0, 0, 
						-sin(angle * PI / 180), 0, cos(angle * PI / 180), 0, 
						0, 0, 0, 1);
};

std::string Matrix4x4::toString() {
	return "ux = " + std::to_string(this->ux) + "\tuy = " + std::to_string(this->uy) + "\tuz = " + std::to_string(this->uz) + "\tun = " + std::to_string(this->un) +
		"\nvx = " + std::to_string(this->vx) + "\tvy = " + std::to_string(this->vy) + "\tvz = " + std::to_string(this->vz) + "\tvn = " + std::to_string(this->vn) +
		"\nwx = " + std::to_string(this->wx) + "\twy = " + std::to_string(this->wy) + "\twz = " + std::to_string(this->wz) + "\twn = " + std::to_string(this->wn) +
		"\nmx = " + std::to_string(this->mx) + "\tmy = " + std::to_string(this->my) + "\tmz = " + std::to_string(this->mz) + "\tmn = " + std::to_string(this->mn);
};