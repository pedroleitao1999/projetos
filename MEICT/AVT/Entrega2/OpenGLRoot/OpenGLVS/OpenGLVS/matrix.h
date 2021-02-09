#include <iostream>
#include <sstream>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vector.h"

class Matrix2x2
{
public:
	Matrix2x2 operator+(const Matrix2x2& a);
	Matrix2x2 operator-(const Matrix2x2& a);
	Vector2d operator*(const Vector2d& vec);
	Matrix2x2 operator*(const Matrix2x2& a);
	Matrix2x2 operator*(const double k);
	Matrix2x2(double ux, double uy, double vx, double vy);
	Matrix2x2 transpose(void);
	Matrix2x2 inverse(void);
	double determinant(void);
	void setUx(double a) { ux = a; };
	void setUy(double b) { uy = b; };
	void setVx(double c) { vx = c; };
	void setVy(double d) { vy = d; };
	double getUx(void) { return ux; };
	double getUy(void) { return uy; };
	double getVx(void) { return vx; };
	double getVy(void) { return vy; };

private:
	double ux, uy;
	double vx, vy;
};

class Matrix3x3
{
public:
	void setUx(double a) { ux = a; };
	void setUy(double b) { uy = b; };
	void setUz(double c) { uz = c; };
	void setVx(double d) { vx = d; };
	void setVy(double e) { vy = e; };
	void setVz(double f) { vz = f; };
	void setWx(double g) { wx = g; };
	void setWy(double h) { wy = h; };
	void setWz(double i) { wz = i; };
	double getUx(void) { return ux; };
	double getUy(void) { return uy; };
	double getUz(void) { return uz; };
	double getVx(void) { return vx; };
	double getVy(void) { return vy; };
	double getVz(void) { return vz; };
	double getWx(void) { return wx; };
	double getWy(void) { return wy; };
	double getWz(void) { return wz; };
	Matrix3x3 operator+(const Matrix3x3& a);
	Matrix3x3 operator-(const Matrix3x3& a);
	Matrix3x3 operator*(const Matrix3x3& a);
	Matrix3x3 operator*(const double k);
	Vector3d operator*(const Vector3d& vec);
	Matrix3x3(double a, double b, double c, double d, double e, double f, double g, double h, double i);
	Matrix3x3 transpose(void);
	Matrix3x3 inverse(void);
	double determinant(void);
	static Vector3d RRF(Vector3d v, Vector3d a, double angle);
	std::string toString(void);

private:
	double ux, uy, uz;
	double vx, vy, vz;
	double wx, wy, wz;
};

class Matrix4x4
{
public:
	void setUx(double a) { ux = a; };
	void setUy(double b) { uy = b; };
	void setUz(double c) { uz = c; };
	void setUn(double d) { un = d; };
	void setVx(double e) { vx = e; };
	void setVy(double f) { vy = f; };
	void setVz(double g) { vz = g; };
	void setVn(double h) { vn = h; };
	void setWx(double i) { wx = i; };
	void setWy(double j) { wy = j; };
	void setWz(double k) { wz = k; };
	void setWn(double l) { wn = l; };
	void setMx(double m) { mx = m; };
	void setMy(double n) { my = n; };
	void setMz(double o) { mz = o; };
	void setMn(double p) { mn = p; };
	double getUx(void) { return ux; };
	double getUy(void) { return uy; };
	double getUz(void) { return uz; };
	double getUn(void) { return un; };
	double getVx(void) { return vx; };
	double getVy(void) { return vy; };
	double getVz(void) { return vz; };
	double getVn(void) { return vn; };
	double getWx(void) { return wx; };
	double getWy(void) { return wy; };
	double getWz(void) { return wz; };
	double getWn(void) { return wn; };
	double getMx(void) { return mx; };
	double getMy(void) { return my; };
	double getMz(void) { return mz; };
	double getMn(void) { return mn; };
	Matrix4x4(double a, double b, double c, double d, 
				double e, double f, double g, double h, 
				double i, double j, double k, double l, 
				double m, double n, double o, double p);
	Matrix4x4 operator+(const Matrix4x4& a);
	Matrix4x4 operator-(const Matrix4x4& a);
	Matrix4x4 operator*(const Matrix4x4& a);
	Matrix4x4 operator*(const double k);
	Vector4d operator*(const Vector4d& vec);
	Matrix4x4 transpose(void);
	static Matrix4x4 translation(double a, double b, double c);
	static Matrix4x4 scale(double a, double b, double c);
	static Matrix4x4 rotationY(double angle);
	std::string toString(void);

private:
	double ux, uy, uz, un;
	double vx, vy, vz, vn;
	double wx, wy, wz, wn;
	double mx, my, mz, mn;
};
