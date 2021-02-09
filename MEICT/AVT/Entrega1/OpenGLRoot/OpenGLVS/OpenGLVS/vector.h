#include <iostream>
#include <sstream>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef DATE_H
#define DATE_H

class Vector2d
{
public:
	void setX(double a);
	void setY(double b);
	double getX(void);
	double getY(void);
	Vector2d(double a, double b);
private:
	double x;
	double y;
};

class Vector3d
{
public:
	void operator=(const Vector3d& vec);
	Vector3d operator+(const Vector3d& vec);
	Vector3d operator-(const Vector3d& vec);
	Vector3d operator*(const double k);
	static double dotProduct(Vector3d vec1, Vector3d vec2);
	Vector3d operator*(const Vector3d& vec);
	bool operator==(const Vector3d& vec);
	bool operator!=(const Vector3d& vec);
	void setX(double a);
	void setY(double b);
	void setZ(double c);
	double getX(void);
	double getY(void);
	double getZ(void);
	Vector3d(double a, double b, double c);
	void assign(Vector3d);
	double distance(void);
	Vector3d normalize(void);
	static double quadruple(Vector3d a, Vector3d b, Vector3d c, Vector3d d);
	friend Vector3d operator*(const double k, const Vector3d& vec);

private:
	double x;
	double y;
	double z;
};

class Vector4d
{
public:
	Vector3d convertion(void);
	void setX(double a);
	void setY(double b);
	void setZ(double c);
	void setW(double d);
	double getX(void);
	double getY(void);
	double getZ(void);
	double getW(void);
	Vector4d(double a, double b, double c, double d);
	void assign(Vector4d);
private:
	double x;
	double y;
	double z;
	double w;
};

#endif

