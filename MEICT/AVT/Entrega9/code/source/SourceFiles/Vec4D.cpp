#include "../HeaderFiles/Vec4D.h"

Vec4D::Vec4D(float x, float y, float z, float w) {
	SetVector(x, y, z, w);
}

Vec4D::Vec4D() {
	SetVector(0.0, 0.0, 0.0, 0.0);
}

void Vec4D::SetVector(float x, float y, float z, float w) {
	v_x = x;
	v_y = y;
	v_z = z;
	v_w = w;
}

float Vec4D::getX() {
	return v_x;
}

float Vec4D::getY() {
	return v_y;
}
float Vec4D::getZ() {
	return v_z;
}

float Vec4D::getW() {
	return v_w;
}

Vec4D Vec4D::add(Vec4D b) {
	float x = getX() + b.getX();
	float y = getY() + b.getY();
	float z = getZ() + b.getZ();
	float w = getW() + b.getW();
	Vec4D v(x, y, z, w);
	return v;
}

bool Vec4D::operator==(Vec4D b) {
	float epsilon = 0.00001f;
	return (getX() - b.getX() < epsilon) && (getY() - b.getY() < epsilon) && (getZ() - b.getZ() < epsilon) && (getW() - b.getW() < epsilon);
}

bool Vec4D::operator!=(Vec4D b) {
	float epsilon = 0.00001f;
	return (getX() - b.getX() > epsilon) || (getY() - b.getY() > epsilon) || (getZ() - b.getZ() > epsilon) || (getW() - b.getW() > epsilon);
}

Vec4D Vec4D::operator+(Vec4D b) {
	return add(b);
}

Vec4D Vec4D::operator-(Vec4D b) {
	return sub(b);
}

Vec4D Vec4D::operator*(float b) {
	return mul(b);
}

Vec4D Vec4D::sub(Vec4D b) {
	float x = getX() - b.getX();
	float y = getY() - b.getY();
	float z = getZ() - b.getZ();
	float w = getW() - b.getW();
	Vec4D v(x, y, z, w);
	return v;
}

Vec4D Vec4D::mul(float b) {
	float x = getX() * b;
	float y = getY() * b;
	float z = getZ() * b;
	float w = getW() * b;
	Vec4D v(x, y, z, w);
	return v;
}

Vec4D Vec4D::normalize() {
	float x = getX() * getX();
	float y = getY() * getY();
	float z = getZ() * getZ();
	float w = getW() * getW();
	float result1 = x + y + z + w;
	float result = sqrt(result1);
	Vec4D normalized(getX() / result, getY() / result, getZ() / result, getW()/result);
	return normalized;
}

float Vec4D::dot(Vec4D b) {
	float x = getX() * b.getX();
	float y = getY() * b.getY();
	float z = getZ() * b.getZ();
	float w = getW() * b.getW();
	return x + y + z + w;
}

Vec4D operator*(float a, Vec4D b) {
    return b.mul(a);
}

float Vec4D::operator[](int i) {
	switch (i) {
	case 0: return v_x;
	case 1: return v_y;
	case 2: return v_z;
	case 3: return v_w;
	default: throw std::invalid_argument("Dimension out of bounds in 2D Vector");
	}
}

std::ostream& operator<<(std::ostream& a, Vec4D b) {
    return a << "(" << b.getX() << " " << b.getY() << " " << b.getZ() << " " << b.getW() << ")";
}

std::vector<GLfloat> Vec4D::toGLfloats() {
    std::vector<GLfloat> glf({(*this)[0], (*this)[1], (*this)[2], (*this)[3]});
    return glf;
}