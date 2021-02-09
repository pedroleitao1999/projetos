#include "../HeaderFiles/Vec4D.h"
#include "../HeaderFiles/Matrix4D.h"
#include <math.h>

Matrix4D::Matrix4D(vector<Vec4D> v) {
	SetMatrix(v);
}

Matrix4D::Matrix4D(vector<float> f) {
	SetMatrix(f);
}

Matrix4D::Matrix4D() {
    *this = Matrix4D::identity();
}


void Matrix4D::SetMatrix(vector<Vec4D> v) {
	if (v.size() != 4) throw invalid_argument("Invalid number of rows in 4x4 matrix");
	vectors = vector<Vec4D>(v);
}

void Matrix4D::SetMatrix(vector<float> f) {
	if (f.size() != 16) throw invalid_argument("Invalid number of values in 4x4 matrix");
	Vec4D row1(f[0], f[1], f[2], f[3]);
	Vec4D row2(f[4], f[5], f[6], f[7]);
	Vec4D row3(f[8], f[9], f[10], f[11]);
	Vec4D row4(f[12], f[13], f[14], f[15]);
	SetMatrix({ row1, row2, row3, row4 });
}

Matrix4D Matrix4D::identity() {
	return Matrix4D({ 1,0,0,0,
					 0,1,0,0,
					 0,0,1,0,
					 0,0,0,1 });
}

Matrix4D Matrix4D::scale(float sx, float sy, float sz) {
	return Matrix4D({ sx,0,0,0,
					 0,sy,0,0,
					 0,0,sz,0,
					 0,0,0,1 });
}

Matrix4D Matrix4D::translate(float dx, float dy, float dz) {
	return Matrix4D({ 1,0,0,dx,
					 0,1,0,dy,
					 0,0,1,dz,
					 0,0,0,1 });
}

double convertToRadians(float degree) {
	double pi = 3.14159265359;
	return (degree * (pi / 180));

}

Matrix4D Matrix4D::rotate(float ang, char axis) {
	float angle = (float)convertToRadians(ang);
	switch (axis) {
	case 'z':
		return Matrix4D({ cos(angle),-sin(angle),0,0,
						 sin(angle),cos(angle),0,0,
						 0,0,1,0,
						 0,0,0,1 });
	case 'y':
		return Matrix4D({ cos(angle),0,sin(angle),0,
						 0,1,0,0,
						 -sin(angle),0,cos(angle),0,
						 0,0,0,1 });
	case 'x':
		return Matrix4D({ 1,0,0,0,
						 0,cos(angle),-sin(angle),0,
						 0,sin(angle),cos(angle),0,
						 0,0,0,1 });
	default:
		throw invalid_argument("Not an axis");
	}
}



Vec4D Matrix4D::getVec(int n) {
	return vectors[n];
}


Matrix4D Matrix4D::add(Matrix4D b) {
	return Matrix4D({ (*this)[0] + b[0], (*this)[1] + b[1], (*this)[2] + b[2], (*this)[3] + b[3] });
}

Matrix4D Matrix4D::operator+(Matrix4D b) {
	return add(b);
}

Matrix4D Matrix4D::operator-(Matrix4D b) {
	return sub(b);
}

Matrix4D Matrix4D::sub(Matrix4D b) {
	return Matrix4D({ (*this)[0] - b[0], (*this)[1] - b[1], (*this)[2] - b[2], (*this)[3] - b[3] });

}


Matrix4D Matrix4D::operator*(float b) {
	return mul(b);
}

Vec4D Matrix4D::operator*(Vec4D b) {
	return mul(b);
}

Matrix4D Matrix4D::operator*(Matrix4D b) {
	return mul(b);
}

Matrix4D Matrix4D::mul(float b) {
	return Matrix4D({ (*this)[0] * b, (*this)[1] * b, (*this)[2] * b, (*this)[3] * b });
}


Vec4D Matrix4D::mul(Vec4D b) {
	return Vec4D((*this)[0][0] * b[0] + (*this)[0][1] * b[1] + (*this)[0][2] * b[2] + (*this)[0][3] * b[3], (*this)[1][0] * b[0] + (*this)[1][1] * b[1] + (*this)[1][2] * b[2] + (*this)[1][3] * b[3], (*this)[2][0] * b[0] + (*this)[2][1] * b[1] + (*this)[2][2] * b[2] + (*this)[2][3] * b[3], (*this)[3][0] * b[0] + (*this)[3][1] * b[1] + (*this)[3][2] * b[2] + (*this)[3][3] * b[3]);
}

Matrix4D Matrix4D::mul(Matrix4D b) {
	Vec4D a1((*this)[0][0], (*this)[0][1], (*this)[0][2], (*this)[0][3]); // row 1 vector of matrix a
	Vec4D a2((*this)[1][0], (*this)[1][1], (*this)[1][2], (*this)[1][3]); // row 2 vector of matrix a
	Vec4D a3((*this)[2][0], (*this)[2][1], (*this)[2][2], (*this)[2][3]); // row 3 vector of matrix a
	Vec4D a4((*this)[3][0], (*this)[3][1], (*this)[3][2], (*this)[3][3]); // row 4 vector of matrix a
	Vec4D b1(b[0][0], b[1][0], b[2][0], b[3][0]); // col 1 vector of matrix b
	Vec4D b2(b[0][1], b[1][1], b[2][1], b[3][1]); // col 2 vector of matrix b
	Vec4D b3(b[0][2], b[1][2], b[2][2], b[3][2]); // col 3 vector of matrix b
	Vec4D b4(b[0][3], b[1][3], b[2][3], b[3][3]); // col 4 vector of matrix b

	return Matrix4D({ a1.dot(b1),a1.dot(b2),a1.dot(b3),a1.dot(b4),a2.dot(b1),a2.dot(b2),a2.dot(b3),a2.dot(b4),a3.dot(b1),a3.dot(b2),a3.dot(b3),a3.dot(b4),a4.dot(b1),a4.dot(b2),a4.dot(b3),a4.dot(b4) });
}

Vec4D Matrix4D::operator[](int i) {
	return getVec(i);
}


Matrix4D Matrix4D::transpose() {
	return Matrix4D({ (*this)[0][0],(*this)[1][0],(*this)[2][0],(*this)[3][0] , (*this)[1][0],(*this)[1][1], (*this)[1][2], (*this)[1][3], (*this)[2][0], (*this)[2][1], (*this)[2][2], (*this)[2][3], (*this)[3][0], (*this)[3][1], (*this)[3][2], (*this)[3][3] });

}

bool Matrix4D::operator==(Matrix4D b) {
	return (*this)[0] == b[0] && (*this)[1] == b[1] && (*this)[2] == b[2] && (*this)[3] == b[3];
}

bool Matrix4D::operator!=(Matrix4D b) {
	return (*this)[0] != b[0] || (*this)[1] != b[1] || (*this)[2] != b[2] || (*this)[3] != b[3];
}


std::ostream& operator<<(std::ostream& a, Matrix4D b) {
	Vec4D first = b[0];
	Vec4D second = b[1];
	Vec4D third = b[2];
	Vec4D fourth = b[3];
	return a << "|" << first.getX() << " " << first.getY() << " " << first.getZ() << " " << first.getW() << "|\n" <<
		"|" << second.getX() << " " << second.getY() << " " << second.getZ() << " " << second.getW() << "|\n" <<
		"|" << third.getX() << " " << third.getY() << " " << third.getZ() << " " << third.getW() << "|\n" <<
		"|" << fourth.getX() << " " << fourth.getY() << " " << fourth.getZ() << " " << fourth.getW() << "|";
}

Matrix4D operator*(float a, Matrix4D b) {
	return b.mul(a);
}

GLfloat* Matrix4D::toGLfloats() {
	GLfloat* gl = new GLfloat[16]{ (*this)[0][0], (*this)[1][0], (*this)[2][0], (*this)[3][0],
						(*this)[0][1], (*this)[1][1], (*this)[2][1], (*this)[3][1],
						(*this)[0][2], (*this)[1][2], (*this)[2][2], (*this)[3][2],
						(*this)[0][3], (*this)[1][3], (*this)[2][3], (*this)[3][3] };
	return gl;
}
