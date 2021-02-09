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
	Matrix2x2 operator*(const GLfloat k);
	Matrix2x2(GLfloat ux, GLfloat uy, GLfloat vx, GLfloat vy);
	Matrix2x2 transpose(void);
	Matrix2x2 inverse(void);
	GLfloat determinant(void);
	void setUx(GLfloat a) { ux = a; };
	void setUy(GLfloat b) { uy = b; };
	void setVx(GLfloat c) { vx = c; };
	void setVy(GLfloat d) { vy = d; };
	GLfloat getUx(void) { return ux; };
	GLfloat getUy(void) { return uy; };
	GLfloat getVx(void) { return vx; };
	GLfloat getVy(void) { return vy; };

private:
	GLfloat ux, uy;
	GLfloat vx, vy;
};

class Matrix3x3
{
public:
	void setUx(GLfloat a) { ux = a; };
	void setUy(GLfloat b) { uy = b; };
	void setUz(GLfloat c) { uz = c; };
	void setVx(GLfloat d) { vx = d; };
	void setVy(GLfloat e) { vy = e; };
	void setVz(GLfloat f) { vz = f; };
	void setWx(GLfloat g) { wx = g; };
	void setWy(GLfloat h) { wy = h; };
	void setWz(GLfloat i) { wz = i; };
	GLfloat getUx(void) { return ux; };
	GLfloat getUy(void) { return uy; };
	GLfloat getUz(void) { return uz; };
	GLfloat getVx(void) { return vx; };
	GLfloat getVy(void) { return vy; };
	GLfloat getVz(void) { return vz; };
	GLfloat getWx(void) { return wx; };
	GLfloat getWy(void) { return wy; };
	GLfloat getWz(void) { return wz; };
	Matrix3x3 operator+(const Matrix3x3& a);
	Matrix3x3 operator-(const Matrix3x3& a);
	Matrix3x3 operator*(const Matrix3x3& a);
	Matrix3x3 operator*(const GLfloat k);
	Vector3d operator*(const Vector3d& vec);
	Matrix3x3(GLfloat a, GLfloat b, GLfloat c, GLfloat d, GLfloat e, GLfloat f, GLfloat g, GLfloat h, GLfloat i);
	Matrix3x3 transpose(void);
	Matrix3x3 inverse(void);
	GLfloat determinant(void);
	static Vector3d RRF(Vector3d v, Vector3d a, GLfloat angle);
	std::string toString(void);

private:
	GLfloat ux, uy, uz;
	GLfloat vx, vy, vz;
	GLfloat wx, wy, wz;
};

class Matrix4x4
{
public:
	void setUx(GLfloat a) { ux = a; };
	void setUy(GLfloat b) { uy = b; };
	void setUz(GLfloat c) { uz = c; };
	void setUn(GLfloat d) { un = d; };
	void setVx(GLfloat e) { vx = e; };
	void setVy(GLfloat f) { vy = f; };
	void setVz(GLfloat g) { vz = g; };
	void setVn(GLfloat h) { vn = h; };
	void setWx(GLfloat i) { wx = i; };
	void setWy(GLfloat j) { wy = j; };
	void setWz(GLfloat k) { wz = k; };
	void setWn(GLfloat l) { wn = l; };
	void setMx(GLfloat m) { mx = m; };
	void setMy(GLfloat n) { my = n; };
	void setMz(GLfloat o) { mz = o; };
	void setMn(GLfloat p) { mn = p; };
	GLfloat getUx(void) { return ux; };
	GLfloat getUy(void) { return uy; };
	GLfloat getUz(void) { return uz; };
	GLfloat getUn(void) { return un; };
	GLfloat getVx(void) { return vx; };
	GLfloat getVy(void) { return vy; };
	GLfloat getVz(void) { return vz; };
	GLfloat getVn(void) { return vn; };
	GLfloat getWx(void) { return wx; };
	GLfloat getWy(void) { return wy; };
	GLfloat getWz(void) { return wz; };
	GLfloat getWn(void) { return wn; };
	GLfloat getMx(void) { return mx; };
	GLfloat getMy(void) { return my; };
	GLfloat getMz(void) { return mz; };
	GLfloat getMn(void) { return mn; };
	Matrix4x4(GLfloat a, GLfloat b, GLfloat c, GLfloat d, 
				GLfloat e, GLfloat f, GLfloat g, GLfloat h, 
				GLfloat i, GLfloat j, GLfloat k, GLfloat l, 
				GLfloat m, GLfloat n, GLfloat o, GLfloat p);
	Matrix4x4 operator*(const Matrix4x4& mat);
	Vector4d operator*(const Vector4d& vec);
	static Matrix4x4 translation(GLfloat a, GLfloat b, GLfloat c);
	static Matrix4x4 scale(GLfloat a, GLfloat b, GLfloat c);
	static Matrix4x4 rotationY(GLfloat angle);
	static Matrix4x4 perspective(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f);
	std::string toString(void);

private:
	GLfloat ux, uy, uz, un;
	GLfloat vx, vy, vz, vn;
	GLfloat wx, wy, wz, wn;
	GLfloat mx, my, mz, mn;
};
