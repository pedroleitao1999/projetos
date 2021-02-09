#include "../HeaderFiles/Vec3D.h"
#include <math.h>

Vec3D::Vec3D(float x, float y, float z) {
	SetVector(x, y, z);
}

Vec3D::Vec3D() {
	SetVector(0.0, 0.0, 0.0);
}


void Vec3D::SetVector(float x, float y, float z) {
	v_x = x;
	v_y = y;
	v_z = z;
}

float Vec3D::getX() {
	return v_x;
}

float Vec3D::getY() {
	return v_y;
}
float Vec3D::getZ() {
	return v_z;
}


Vec3D Vec3D::normalize() {
	float x = getX() * getX();
	float y = getY() * getY();
	float z = getZ() * getZ();
	float result1 = x + y + z;
	float result = sqrt(result1);
	Vec3D normalized(getX() / result, getY() / result, getZ() / result);
	return normalized;
}

double Vec3D::convertToRadians(float degree) {
	double pi = 3.14159265359;
	return (degree * (pi / 180));

}


Vec3D Vec3D::add(Vec3D b) {
	float x = getX() + b.getX();
	float y = getY() + b.getY();
	float z = getZ() + b.getZ();
	Vec3D v(x, y, z);
	return v;
}

Vec3D Vec3D::operator+(Vec3D b) {
	return add(b);
}

bool Vec3D::operator==(Vec3D b) {
	float epsilon = 0.00001f;
	return (getX() - b.getX() < epsilon) && (getY() - b.getY() < epsilon) && (getZ() - b.getZ() < epsilon);
}

bool Vec3D::operator!=(Vec3D b) {
	float epsilon = 0.00001f;
	return (getX() - b.getX() > epsilon) || (getY() - b.getY() > epsilon) || (getZ() - b.getZ() > epsilon);
}

Vec3D Vec3D::operator-(Vec3D b) {
	return sub(b);
}

Vec3D Vec3D::operator*(float b) {
	return mul(b);
}


Vec3D Vec3D::sub(Vec3D b) {
	float x = getX() - b.getX();
	float y = getY() - b.getY();
	float z = getZ() - b.getZ();
	Vec3D v(x, y, z);
	return v;
}

Vec3D Vec3D::mul(float b) {
	float x = getX() * b;
	float y = getY() * b;
	float z = getZ() * b;
	Vec3D v(x, y, z);
	return v;
}

float Vec3D::dot(Vec3D b) {
	float x = getX() * b.getX();
	float y = getY() * b.getY();
	float z = getZ() * b.getZ();
	return x + y + z;
}

Vec3D Vec3D::cross(Vec3D b) {
	float x = getY() * b.getZ() - getZ() * b.getY();
	float y = getZ() * b.getX() - getX() * b.getZ();
	float z = getX() * b.getY() - getY() * b.getX();
	Vec3D v(x, y, z);
	return v;
}

Vec3D Vec3D::rotationFormula(float degree, Vec3D k) {
	double theta = convertToRadians(degree);
	Vec3D normalizedVecK = k.normalize();
	float cosTheta = (float)cos(theta);
	float sinTheta = (float)sin(theta);
	Vec3D v(getX(), getY(), getZ());
	Vec3D rotVecAux1(mul(cosTheta));
	Vec3D rotVecAux2(normalizedVecK.cross(v).mul(sinTheta));
	Vec3D rotVecAux3(rotVecAux1.add(rotVecAux2));
	Vec3D rotVecAux4(normalizedVecK.mul(dot(normalizedVecK)));
	Vec3D rotVecAux5(rotVecAux4.mul(1 - cosTheta));
	Vec3D rotVec(rotVecAux3.add(rotVecAux5));
	return rotVec;
}

float Vec3D::operator[](int i) {
    switch(i) {
        case 0: return v_x;
        case 1: return v_y;
        case 2: return v_z;
        default: throw std::invalid_argument("Dimension out of bounds in 3D Vector");
    }
}

Vec3D operator*(float a, Vec3D b) {
	return b.mul(a);
}

std::ostream& operator<<(std::ostream& a, Vec3D b) {
    return a << "(" << b.getX() << " " << b.getY() << " " << b.getZ() << ")";
}