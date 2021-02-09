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

Matrix2x2::Matrix2x2(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
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

Matrix2x2 Matrix2x2::operator*(const GLfloat k) {
	Matrix2x2 mat = Matrix2x2(0, 0, 0, 0);
	mat.ux = this->ux * k;
	mat.uy = this->uy * k;
	mat.vx = this->vx * k;
	mat.vy = this->vy * k;
	return mat;
};

Vector2d Matrix2x2::operator*(const Vector2d& vec) {
	Vector2d v = vec;
	GLfloat a = v.getX() * this->ux + v.getY() * this->uy;
	GLfloat b = v.getX() * this->vx + v.getY() * this->vy;
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

GLfloat Matrix2x2::determinant(void) {
	return (this->ux * this->vy) - (this->uy * this->vx);
};


Matrix2x2 Matrix2x2::inverse(void) {
	GLfloat det = Matrix2x2::determinant();

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

Matrix3x3::Matrix3x3(GLfloat a, GLfloat b, GLfloat c, GLfloat d, GLfloat e, GLfloat f, GLfloat g, GLfloat h, GLfloat i)
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

Matrix3x3 Matrix3x3::operator*(const GLfloat k) {
	Matrix3x3 mat = Matrix3x3(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
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
	GLfloat a = v.getX() * this->ux + v.getY() * this->uy + v.getZ() * this->uz;
	GLfloat b = v.getX() * this->vx + v.getY() * this->vy + v.getZ() * this->vz;
	GLfloat c = v.getX() * this->wx + v.getY() * this->wy + v.getZ() * this->wz;
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

GLfloat Matrix3x3::determinant(void) {
	GLfloat res1 = this->ux * (this->vy * this->wz - this->vz * this->wy);
	GLfloat res2 = this->uy * (this->vx * this->wz - this->vz * this->wx);
	GLfloat res3 = this->uz * (this->vx * this->wy - this->vy * this->wx);
	return abs(res1 - res2 + res3);
};


Matrix3x3 Matrix3x3::inverse(void) {
	GLfloat det = Matrix3x3::determinant();

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



Vector3d Matrix3x3::RRF(Vector3d v, Vector3d a, GLfloat angle) {
	a = a.normalize();
	GLfloat num = Vector3d::dotProduct(a, v);
	return v + (a * v) * sin(angle * (GLfloat)PI / 180.0f) + ((a * num) - v) * (1 - cos(angle * (GLfloat)PI / 180.0f));
};

std::string Matrix3x3::toString() {
	return "ux = " + std::to_string(this->ux) + "\tuy = " + std::to_string(this->uy) + "\tuz = " + std::to_string(this->uz) +
		"\nvx = " + std::to_string(this->vx) + "\tvy = " + std::to_string(this->vy) + "\tvz = " + std::to_string(this->vz) +
		"\nwx = " + std::to_string(this->wx) + "\twy = " + std::to_string(this->wy) + "\twz = " + std::to_string(this->wz);
};

Matrix4x4::Matrix4x4(GLfloat a, GLfloat b, GLfloat c, GLfloat d, 
					GLfloat e, GLfloat f, GLfloat g, GLfloat h, 
					GLfloat i, GLfloat j, GLfloat k, GLfloat l, 
					GLfloat m, GLfloat n, GLfloat o, GLfloat p)
{
	ux = a; uy = b; uz = c; un = d;
	vx = e; vy = f; vz = g; vn = h;
	wx = i; wy = j; wz = k; wn = l;
	mx = m; my = n; mz = o; mn = p;
};

Matrix4x4 Matrix4x4::translation(GLfloat a, GLfloat b, GLfloat c) {
	return Matrix4x4(1.0f, 0.0f, 0.0f, a, 0.0f, 1.0f, 0.0f, b, 0.0f, 0.0f, 1.0f, c, 0.0f, 0.0f, 0.0f, 1.0f);
};

Matrix4x4 Matrix4x4::scale(GLfloat a, GLfloat b, GLfloat c) {
	return Matrix4x4(a, 0.0f, 0.0f, 0.0f,
					0.0f, b, 0.0f, 0.0f,
					0.0f, 0.0f, c, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
};

Matrix4x4 Matrix4x4::rotationY(GLfloat angle) {
	return Matrix4x4(cos(angle * (GLfloat)PI / 180.0f), 0.0f, sin(angle * (GLfloat)PI / 180.0f), 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f, 
						-sin(angle * (GLfloat)PI / 180.0f), 0.0f, cos(angle * (GLfloat)PI / 180.0f), 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);
};

Matrix4x4 Matrix4x4::perspective(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f) {
	GLfloat d = (1.0f / tan(fov * (GLfloat)PI / 180.0f / 2.0f));
	return Matrix4x4(
		(GLfloat) d/aspect, 0.0f, 0.0f, 0.0f,
		0.0f, d, 0.0f, 0.0f,
		0.0f, 0.0f,	(GLfloat) (n + f) / (n - f), (GLfloat) 2.0f * n * f /(n - f),
		0.0f, 0.0f, -1.0f, 0.0f
	);
};

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) {
	
	GLfloat a = this->ux * mat.ux + this->uy * mat.vx + this->uz * mat.wx + this->un * mat.mx;
	GLfloat b = this->ux * mat.uy + this->uy * mat.vy + this->uz * mat.wy + this->un * mat.my;
	GLfloat c = this->ux * mat.uz + this->uy * mat.vz + this->uz * mat.wz + this->un * mat.mz;
	GLfloat d = this->ux * mat.un + this->uy * mat.vn + this->uz * mat.wn + this->un * mat.mn;

	GLfloat e = this->vx * mat.ux + this->vy * mat.vx + this->vz * mat.wx + this->vn * mat.mx;
	GLfloat f = this->vx * mat.uy + this->vy * mat.vy + this->vz * mat.wy + this->vn * mat.my;
	GLfloat g = this->vx * mat.uz + this->vy * mat.vz + this->vz * mat.wz + this->vn * mat.mz;
	GLfloat h = this->vx * mat.un + this->vy * mat.vn + this->vz * mat.wn + this->vn * mat.mn;

	GLfloat i = this->wx * mat.ux + this->wy * mat.vx + this->wz * mat.wx + this->wn * mat.mx;
	GLfloat j = this->wx * mat.uy + this->wy * mat.vy + this->wz * mat.wy + this->wn * mat.my;
	GLfloat k = this->wx * mat.uz + this->wy * mat.vz + this->wz * mat.wz + this->wn * mat.mz;
	GLfloat l = this->wx * mat.un + this->wy * mat.vn + this->wz * mat.wn + this->wn * mat.mn;

	GLfloat m = this->mx * mat.ux + this->my * mat.vx + this->mz * mat.wx + this->mn * mat.mx;
	GLfloat n = this->mx * mat.uy + this->my * mat.vy + this->mz * mat.wy + this->mn * mat.my;
	GLfloat o = this->mx * mat.uz + this->my * mat.vz + this->mz * mat.wz + this->mn * mat.mz;
	GLfloat p = this->mx * mat.un + this->my * mat.vn + this->mz * mat.wn + this->mn * mat.mn;

	Matrix4x4 matrix = Matrix4x4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p);

	return matrix;
};

Vector4d Matrix4x4::operator*(const Vector4d& vec) {
	Vector4d v = vec;
	GLfloat a = v.getX() * this->ux + v.getY() * this->uy + v.getZ() * this->uz + v.getW() * this->un;
	GLfloat b = v.getX() * this->vx + v.getY() * this->vy + v.getZ() * this->vz + v.getW() * this->vn;
	GLfloat c = v.getX() * this->wx + v.getY() * this->wy + v.getZ() * this->wz + v.getW() * this->wn;
	GLfloat d = v.getX() * this->mx + v.getY() * this->my + v.getZ() * this->mz + v.getW() * this->mn;
	Vector4d res = Vector4d(a, b, c, d);
	return res;
};

std::string Matrix4x4::toString() {
	return "ux = " + std::to_string(this->ux) + "\tuy = " + std::to_string(this->uy) + "\tuz = " + std::to_string(this->uz) + "\tun = " + std::to_string(this->un) +
		"\nvx = " + std::to_string(this->vx) + "\tvy = " + std::to_string(this->vy) + "\tvz = " + std::to_string(this->vz) + "\tvn = " + std::to_string(this->vn) +
		"\nwx = " + std::to_string(this->wx) + "\twy = " + std::to_string(this->wy) + "\twz = " + std::to_string(this->wz) + "\twn = " + std::to_string(this->wn) +
		"\nmx = " + std::to_string(this->mx) + "\tmy = " + std::to_string(this->my) + "\tmz = " + std::to_string(this->mz) + "\tmn = " + std::to_string(this->mn);
};
