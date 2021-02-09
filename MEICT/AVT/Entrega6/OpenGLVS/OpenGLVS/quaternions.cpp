#include "main.h"

///////////////////// Complex Numbers /////////////////////

ComplexNumber2d::ComplexNumber2d(GLfloat a, GLfloat b) {
	real = a;
	imaginary = b;
}

const ComplexNumber2d ComplexNumber2d::transpose(ComplexNumber2d& cn) {
	const ComplexNumber2d tra = ComplexNumber2d(cn.getReal(), -cn.getImaginary());
	return tra;
}

bool ComplexNumber2d::operator==(const ComplexNumber2d& cn) {
	return (this->real == cn.real) && (this->imaginary == cn.imaginary);
}

bool ComplexNumber2d::isReal() {
	return *this == transpose(*this);
}

ComplexNumber2d ComplexNumber2d::operator+(const ComplexNumber2d& cn) {
	GLfloat a = this->real + cn.real;
	GLfloat b = this->imaginary + cn.imaginary;
	ComplexNumber2d sum = ComplexNumber2d(a, b);
	return sum;
}

ComplexNumber2d ComplexNumber2d::operator*(const ComplexNumber2d& cn) {
	GLfloat a = this->real * cn.real - this->imaginary * cn.imaginary;
	GLfloat b = this->real * cn.imaginary + cn.real * this->imaginary;
	ComplexNumber2d mul = ComplexNumber2d(a, b);
	return mul;
}

GLfloat ComplexNumber2d::module(ComplexNumber2d cn) {
	GLfloat mod = (GLfloat) sqrt(cn.getReal() * cn.getReal() + cn.getImaginary() * cn.getImaginary());
	return mod;
}

Matrix2x2 ComplexNumber2d::toMatrix() {
	Matrix2x2 mat = Matrix2x2(
		this->real, -this->imaginary,
		this->imaginary, this->real
	);
	return mat;
}

/////////////////////// Quaternions ///////////////////////

Quaternion::Quaternion() {
	t = 0;
	v = Vector3d();
}

Quaternion::Quaternion(GLfloat tQ, Vector3d vec) {
	t = tQ;
	v = vec;
}

bool Quaternion::isReal() {
	return this->v.getX() == 0 &&
		this->v.getY() == 0 &&
		this->v.getZ() == 0;
}

bool Quaternion::isPure() {
	return this->t == 0;
}

Quaternion operator*(const GLfloat lambda, const Quaternion& q) {
	GLfloat newT = (GLfloat) lambda * q.t;
	Vector3d newVec = lambda * q.v;
	Quaternion res = Quaternion(newT, newVec);
	return res;
}

Quaternion Quaternion::operator+(const Quaternion& q) {
	GLfloat newT = this->t + q.t;
	Vector3d newVec = this->v + q.v;
	Quaternion res = Quaternion(newT, newVec);
	return res;
}

Quaternion Quaternion::operator*(const Quaternion& q) {
	GLfloat newT = this->t * q.t - Vector3d::dotProduct(this->v, q.v);
	Vector3d newVec = this->t * q.v + q.t * this->v + this->v * q.v;
	Quaternion res = Quaternion(newT, newVec);
	return res;
}
const Quaternion Quaternion::transpose(Quaternion& q) {
	Vector3d newVec = -1 * q.v;
	Quaternion res = Quaternion(q.t, newVec);
	return res;
}

Quaternion Quaternion::dotProduct(Quaternion q0, Quaternion q1) {
	Quaternion res = 0.5f * ( q0 * transpose(q1) + q1 * transpose(q0) );
	return res;
}

GLfloat Quaternion::module(Quaternion& q) {
	GLfloat vecX = q.v.getX();
	GLfloat vecY = q.v.getY();
	GLfloat vecZ = q.v.getZ();
	GLfloat mod = sqrt(q.t * q.t + vecX * vecX + vecY * vecY + vecZ * vecZ);
	return mod;
}

Quaternion Quaternion::qFunction() {
	Quaternion res = *this * transpose(*this);
	return res;
}

Quaternion Quaternion::inverse(Quaternion& q) {/*
	Quaternion inv = transpose(q) / q.qFunction();
	return inv;*/
	return q;
}

Matrix2x2 Quaternion::toMatrix2x2() {
	return Matrix2x2(1, 1, 1, 1);
}

Matrix4x4 Quaternion::toMatrix4x4() {
	Matrix4x4 mat = Matrix4x4(
		this->t, -this->v.getZ(), this->v.getY(), this->v.getX(),
		this->v.getZ(), this->t, -this->v.getX(), this->v.getY(),
		-this->v.getY(), this->v.getX(), this->t, this->v.getZ(),
		-this->v.getX(), -this->v.getY(), -this->v.getZ(), this->t
	);
	return mat;
}

Quaternion Quaternion::normalize(Quaternion& q) {
	GLfloat mod = module(q);
	GLfloat newT = (GLfloat) q.t / mod;
	Vector3d newVec = q.v * ((GLfloat) 1 / mod);
	Quaternion res = Quaternion(newT, newVec);
	return res;
}

Vector3d Quaternion::getAxis() {
	GLfloat div = (GLfloat) 1 / sqrt(1 - this->t * this->t);
	Vector3d a = this->v * div;
	return a;
}

GLfloat Quaternion::getAngle() {
	GLfloat angle = (GLfloat) 2 * acos(this->t);
	return angle;
}

Matrix3x3 Quaternion::toMatrix3x3() {
	GLfloat vecX = this->v.getX();
	GLfloat vecY = this->v.getY();
	GLfloat vecZ = this->v.getZ();
	Matrix3x3 mat = Matrix3x3(
		1-2*vecY*vecY-2*vecZ*vecZ, 2*vecX*vecY-2*this->t*vecZ, 2*vecX*vecZ+2*this->t*vecY,
		2*vecX*vecY+2*this->t*vecZ, 1-2*vecX*vecX-2*vecZ*vecZ, 2*vecY*vecZ-2*this->t*vecX,
		2*vecX*vecZ-2*this->t*vecY, 2*vecY*vecZ+2*this->t*vecX, 1-2*vecX*vecX-2*vecY*vecY
	);
	return mat;
}

Quaternion Quaternion::lerp(GLfloat t, Quaternion q0, Quaternion q1) {
	Quaternion res = (1.0f - t) * q0 + t * q1;
	return res;
}

Quaternion Quaternion::slerp(GLfloat t, Quaternion q0, Quaternion q1) {
	GLfloat cosAngle = q0.t*q1.t + q0.v.getX()*q1.v.getX() + q0.v.getY()*q1.v.getY() + q0.v.getZ()*q1.v.getZ();
	GLfloat angle = (GLfloat) acos(cosAngle*DEGREES_TO_RADIANS);
	GLfloat div = (GLfloat) 1.0f / sin(angle);
	Quaternion res = (GLfloat) sin((1.0f - t) * angle) * div * q0 + sin(t * angle) * div * q1;
	return res;
}

Quaternion Quaternion::toQuaternion(Vector3d axis, GLfloat angle) {
	GLfloat t = (GLfloat) cos(angle / 2);
	Vector3d v = (GLfloat) sin(angle / 2) * axis;
	Quaternion q = Quaternion(t, v);
	return q;
}

