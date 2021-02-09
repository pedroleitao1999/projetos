#include "../HeaderFiles/Vec2D.h"

Vec2D::Vec2D(float x, float y) {
	SetVector(x, y);
}

void Vec2D::SetVector(float x, float y) {
	v_x = x;
	v_y = y;
}

float Vec2D::getX() {
	return v_x;
}

float Vec2D::getY() {
	return v_y;
}

bool Vec2D::operator==(Vec2D b) {
	float epsilon = 0.00001f;
	return (getX() - b.getX() < epsilon) && (getY() - b.getY() < epsilon);
}

bool Vec2D::operator!=(Vec2D b) {
	float epsilon = 0.00001f;
	return (getX() - b.getX() > epsilon) || (getY() - b.getY() > epsilon);
}

Vec2D Vec2D::operator+(Vec2D b) {
	return add(b);
}

Vec2D Vec2D::operator-(Vec2D b) {
	return sub(b);
}

Vec2D Vec2D::operator*(float b) {
	return mul(b);
}

float Vec2D::operator[](int i) {
    switch(i) {
        case 0: return v_x;
        case 1: return v_y;
        default: throw std::invalid_argument("Dimension out of bounds in 2D Vector");
    }
}

Vec2D Vec2D::add(Vec2D b) {
	float x = getX() + b.getX();
	float y = getY() + b.getY();
	Vec2D v(x, y);
	return v;
}

Vec2D Vec2D::mul(float b) {
	float x = getX() * b;
	float y = getY() * b;
	Vec2D v(x, y);
	return v;
}


Vec2D Vec2D::sub(Vec2D b) {
	float x = getX() - b.getX();
	float y = getY() - b.getY();
	Vec2D v(x, y);
	return v;
}

float Vec2D::dot(Vec2D b) {
	float x = getX() * b.getX();
	float y = getY() * b.getY();
	return x + y;
}

Vec2D operator*(float a, Vec2D b) {
    return b.mul(a);
}

std::ostream& operator<<(std::ostream& a, Vec2D b) {
    return a << "(" << b.getX() << " " << b.getY() << ")";
}