#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef DATE_H
#define DATE_H

class Vector2d
{
public:
	void setX(const GLfloat a);
	void setY(const GLfloat b);
	const GLfloat getX(void);
	const GLfloat getY(void);
	Vector2d(GLfloat a, GLfloat b);
	void assign(Vector2d);
	void operator=(const Vector2d& vec);
	Vector2d operator+(const Vector2d& vec);
	Vector2d operator-(const Vector2d& vec);
	Vector2d operator*(const GLfloat k);
	static GLfloat dotProduct(Vector2d vec1, Vector2d vec2);
	bool operator==(const Vector2d& vec);
	bool operator!=(const Vector2d& vec);
	friend Vector2d operator*(const GLfloat k, const Vector2d& vec);
private:
	GLfloat x;
	GLfloat y;
};

class Vector3d
{
public:
	void operator=(const Vector3d& vec);
	Vector3d operator+(const Vector3d& vec);
	Vector3d operator-(const Vector3d& vec);
	Vector3d operator*(const GLfloat k);
	static GLfloat dotProduct(Vector3d vec1, Vector3d vec2);
	Vector3d operator*(const Vector3d& vec);
	bool operator==(const Vector3d& vec);
	bool operator!=(const Vector3d& vec);
	void setX(GLfloat a);
	void setY(GLfloat b);
	void setZ(GLfloat c);
	GLfloat getX(void);
	GLfloat getY(void);
	GLfloat getZ(void);
	Vector3d(GLfloat a, GLfloat b, GLfloat c);
	void assign(Vector3d);
	GLfloat distance(void);
	Vector3d normalize(void);
	static GLfloat quadruple(Vector3d a, Vector3d b, Vector3d c, Vector3d d);
	friend Vector3d operator*(const GLfloat k, const Vector3d& vec);
	std::string toString(void);

private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class Vector4d
{
public:
	Vector3d convertion(void);
	void setX(GLfloat a);
	void setY(GLfloat b);
	void setZ(GLfloat c);
	void setW(GLfloat d);
	GLfloat getX(void);
	GLfloat getY(void);
	GLfloat getZ(void);
	GLfloat getW(void);
	Vector4d(GLfloat a, GLfloat b, GLfloat c, GLfloat d);
	void assign(Vector4d);
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
};

#endif
