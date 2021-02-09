#include "main.h"

// Vector4d

// Convertion function 
Vector3d Vector4d::convertion(void) {
	GLfloat x = (GLfloat)this->x / this->w;
	GLfloat y = (GLfloat)this->y / this->w;
	GLfloat z = (GLfloat)this->z / this->w;
	Vector3d vecResult = Vector3d(x, y, z);
	return vecResult;
};

void Vector4d::setX(GLfloat a)
{
	x = a;
};

void Vector4d::setY(GLfloat b)
{
	y = b;
};

void Vector4d::setZ(GLfloat c)
{
	z = c;
};

void Vector4d::setW(GLfloat d)
{
	w = d;
};

GLfloat Vector4d::getX(void)
{
	return x;
};

GLfloat Vector4d::getY(void)
{
	return y;
};

GLfloat Vector4d::getZ(void)
{
	return z;
};

GLfloat Vector4d::getW(void)
{
	return w;
};

Vector4d::Vector4d(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	x = a;
	y = b;
	z = c;
	w = d;
};


void Vector4d::assign(Vector4d vec)
{
	this->x = vec.getX();
	this->y = vec.getY();
	this->z = vec.getZ();
};

// Vector3d

Vector3d::Vector3d() {
	x = 0;
	y = 0;
	z = 0;
}

Vector3d::Vector3d(GLfloat a, GLfloat b, GLfloat c)
{
	x = a;
	y = b;
	z = c;
};


// Overloading assignment operator (=)
void Vector3d::operator=(const Vector3d& vec) {
	setX(vec.x);
	setY(vec.y);
	setZ(vec.z);
};

// Overloading equality operator (==)
bool Vector3d::operator==(const Vector3d& vec) {
	if (this->x == vec.x && this->y == vec.y && this->z == vec.z) {
		return true;
	}
	return false;
};

// Overloading not-equality operator (!=)
bool Vector3d::operator!=(const Vector3d& vec) {
	if (this->x == vec.x && this->y == vec.y && this->z == vec.z) {
		return false;
	}
	return true;
};

// Overloading sum operator (+)
Vector3d Vector3d::operator+(const Vector3d& vec) {
	Vector3d vecResult = Vector3d(0, 0, 0);
	vecResult.x = this->x + vec.x;
	vecResult.y = this->y + vec.y;
	vecResult.z = this->z + vec.z;
	return vecResult;
};

// Overloading subtract operator (-)
Vector3d Vector3d::operator-(const Vector3d& vec) {
	Vector3d vecResult = Vector3d(0, 0, 0);
	vecResult.x = this->x - vec.x;
	vecResult.y = this->y - vec.y;
	vecResult.z = this->z - vec.z;
	return vecResult;
};

// Overloading multiplication by scalar operator (*)
Vector3d Vector3d::operator*(const GLfloat k) {
	Vector3d vecResult = Vector3d(0.0f, 0.0f, 0.0f);
	vecResult.x = this->x * k;
	vecResult.y = this->y * k;
	vecResult.z = this->z * k;
	return vecResult;
};

// Dot product function (.)
GLfloat Vector3d::dotProduct(Vector3d vec1, Vector3d vec2) {
	GLfloat result = 0.0f;
	result += vec1.getX() * vec2.getX();
	result += vec1.getY() * vec2.getY();
	result += vec1.getZ() * vec2.getZ();
	return result;
};

// Overloading cross product operator (*)
Vector3d Vector3d::operator*(const Vector3d& vec) {
	Vector3d vecResult = Vector3d(0, 0, 0);
	vecResult.x = (this->y * vec.z - this->z * vec.y);
	vecResult.y = (this->z * vec.x - this->x * vec.z);
	vecResult.z = (this->x * vec.y - this->y * vec.x);
	return vecResult;
};

// Distance calculator
GLfloat Vector3d::distance(void) {
	GLfloat x = this->x;
	GLfloat y = this->y;
	GLfloat z = this->z;
	GLfloat d = sqrt(x * x + y * y + z * z);
	return d;
};

// Normalize function 
Vector3d Vector3d::normalize(void) {
	GLfloat d = distance();
	GLfloat x = (GLfloat)this->x / d;
	GLfloat y = (GLfloat)this->y / d;
	GLfloat z = (GLfloat)this->z / d;
	Vector3d vecResult = Vector3d(x, y, z);
	return vecResult;
};

// Quadruple function
GLfloat Vector3d::quadruple(Vector3d a, Vector3d b, Vector3d c, Vector3d d) {
	GLfloat res1 = dotProduct(a, c);
	GLfloat res2 = dotProduct(b, d);
	GLfloat res3 = dotProduct(a, d);
	GLfloat res4 = dotProduct(b, c);
	GLfloat res5 = res1 * res2;
	GLfloat res6 = res3 * res4;
	return (res5 - res6);
};

// Overloading multiplication of scalar operator by vector (*)
Vector3d operator*(const GLfloat k, const Vector3d& vec) {
	Vector3d vecResult = Vector3d(0.0f, 0.0f, 0.0f);
	vecResult.x = k * vec.x;
	vecResult.y = k * vec.y;
	vecResult.z = k * vec.z;
	return vecResult;
};

void Vector3d::setX(GLfloat a)
{
	x = a;
};

void Vector3d::setY(GLfloat b)
{
	y = b;
};

void Vector3d::setZ(GLfloat c)
{
	z = c;
};


GLfloat Vector3d::getX(void)
{
	return x;
};

GLfloat Vector3d::getY(void)
{
	return y;
};

GLfloat Vector3d::getZ(void)
{
	return z;
};

void Vector3d::assign(Vector3d vec)
{
	this->x = vec.getX();
	this->y = vec.getY();
	this->z = vec.getZ();
};

// Vector2d

void Vector2d::setX(const GLfloat a)
{
	x = a;
};

void Vector2d::setY(const GLfloat b)
{
	y = b;
};

const GLfloat Vector2d::getX(void)
{
	return x;
};

const GLfloat Vector2d::getY(void)
{
	return y;
};

Vector2d::Vector2d(const GLfloat a, const GLfloat b)
{
	x = a;
	y = b;
};

void Vector2d::assign(Vector2d vec)
{
	this->x = vec.getX();
	this->y = vec.getY();
};

// Overloading assignment operator (=)
void Vector2d::operator=(const Vector2d& vec) {
	setX(vec.x);
	setY(vec.y);
};

// Overloading equality operator (==)
bool Vector2d::operator==(const Vector2d& vec) {
	if (this->x == vec.x && this->y == vec.y) {
		return true;
	}
	return false;
};

// Overloading not-equality operator (!=)
bool Vector2d::operator!=(const Vector2d& vec) {
	if (this->x == vec.x && this->y == vec.y) {
		return false;
	}
	return true;
};

// Overloading sum operator (+)
Vector2d Vector2d::operator+(const Vector2d& vec) {
	Vector2d vecResult = Vector2d(0.0f, 0.0f);
	vecResult.x = this->x + vec.x;
	vecResult.y = this->y + vec.y;
	return vecResult;
};

// Overloading subtract operator (-)
Vector2d Vector2d::operator-(const Vector2d& vec) {
	Vector2d vecResult = Vector2d(0.0f, 0.0f);
	vecResult.x = this->x - vec.x;
	vecResult.y = this->y - vec.y;
	return vecResult;
};

// Overloading multiplication by scalar operator (*)
Vector2d Vector2d::operator*(const GLfloat k) {
	Vector2d vecResult = Vector2d(0.0f, 0.0f);
	vecResult.x = this->x * k;
	vecResult.y = this->y * k;
	return vecResult;
};

// Dot product function (.)
GLfloat Vector2d::dotProduct(Vector2d vec1, Vector2d vec2) {
	GLfloat result = 0.0f;
	result += vec1.getX() * vec2.getX();
	result += vec1.getY() * vec2.getY();
	return result;
};

// Overloading multiplication of scalar operator by vector (*)
Vector2d operator*(const GLfloat k, const Vector2d& vec) {
	Vector2d vecResult = Vector2d(0.0f, 0.0f);
	vecResult.x = k * vec.x;
	vecResult.y = k * vec.y;
	return vecResult;
};


std::string Vector3d::toString(void) {
	return "x = " + std::to_string(this->x) +
		"\ny = " + std::to_string(this->y) +
		"\nz = " + std::to_string(this->z);
};