#include "../HeaderFiles/Vec2D.h"
#include "../HeaderFiles/Matrix2D.h"

Matrix2D::Matrix2D(vector<Vec2D> v) {
	SetMatrix(v);
}

Matrix2D::Matrix2D(vector<float> f) {
    SetMatrix(f);
}

void Matrix2D::SetMatrix(vector<Vec2D> v) {
    if (v.size() != 2) throw invalid_argument("Invalid number of rows in 2x2 matrix");
    vectors = vector<Vec2D>(v);
}

void Matrix2D::SetMatrix(vector<float> f) {
    if (f.size() != 4) throw invalid_argument("Invalid number of values in 2x2 matrix");
    Vec2D row1(f[0], f[1]);
    Vec2D row2(f[2], f[3]);
    SetMatrix({row1, row2});
}

Matrix2D Matrix2D::identity() {
    return Matrix2D({1,0,
                     0,1});
}

Vec2D Matrix2D::getVec(int i) {
	return vectors[i];
}

Matrix2D Matrix2D::add(Matrix2D b) {
    return Matrix2D( {(*this)[0] + b[0], (*this)[1] + b[1]} );
}

Matrix2D Matrix2D::sub(Matrix2D b) {
    return Matrix2D( {(*this)[0] - b[0], (*this)[1] - b[1]} );
}

Matrix2D Matrix2D::mul(float b) {
    return Matrix2D( {(*this)[0] * b, (*this)[1] * b} );
}

Vec2D Matrix2D::mul(Vec2D b) {
    return Vec2D( (*this)[0][0] * b[0] + (*this)[0][1] * b[1], (*this)[1][0] * b[0] + (*this)[1][1] * b[1] );
}

Matrix2D Matrix2D::mul(Matrix2D b) {
    Vec2D a1( (*this)[0][0], (*this)[0][1] ); // row 1 vector of matrix a
    Vec2D a2( (*this)[1][0], (*this)[1][1] ); // row 2 vector of matrix a
    Vec2D b1( b[0][0], b[1][0]); // col 1 vector of matrix b
    Vec2D b2( b[0][1], b[1][1]); // col 2 vector of matrix b

    return Matrix2D( {a1.dot(b1), a1.dot(b2), a2.dot(b1), a2.dot(b2)} );
}

Matrix2D Matrix2D::operator+(Matrix2D b) {
	return add(b);
}

Matrix2D Matrix2D::operator-(Matrix2D b) {
	return sub(b);
}

Matrix2D Matrix2D::operator*(float b) {
	return mul(b);
}

Vec2D Matrix2D::operator*(Vec2D b) {
	return mul(b);
}

Matrix2D Matrix2D::operator*(Matrix2D b) {
	return mul(b);
}

Vec2D Matrix2D::operator[](int i) {
    return getVec(i);
}

bool Matrix2D::operator==(Matrix2D b) {
    return (*this)[0] == b[0] && (*this)[1] == b[1];
}

bool Matrix2D::operator!=(Matrix2D b) {
    return (*this)[0] != b[0] || (*this)[1] != b[1];
}

Matrix2D Matrix2D::transpose() {
    return Matrix2D( {(*this)[0][0], (*this)[1][0], (*this)[0][1], (*this)[1][1]} );
}

float Matrix2D::det() {
    return (*this)[0][0] * (*this)[1][1] - (*this)[1][0] * (*this)[0][1];
}

Matrix2D Matrix2D::inverse() {
    float determinant = det();
	if (determinant != 0) {
		float scalar = 1 / determinant;
		return (Matrix2D( {(*this)[1][1], -(*this)[0][1], -(*this)[1][0], (*this)[0][0]} ) * scalar);
	} else {
		cerr << "Invalid matrix to be inverted, determinant is zero. Printing the original matrix instead" << endl;
		return *this;
	}
}


std::ostream& operator<<(std::ostream& a, Matrix2D b) {
	Vec2D first = b[0];
	Vec2D second = b[1];
	return a << "|" << first.getX() << " " << first.getY() << "|\n" <<
	"|" << second.getX() << " " << second.getY() << "|";
}

Matrix2D operator*(float a, Matrix2D b) {
	return b.mul(a);
}

GLfloat* Matrix2D::toGLfloats() {
    GLfloat* gl = new GLfloat[4] {(*this)[0][0], (*this)[1][0], (*this)[0][1], (*this)[1][1]};
    return gl;
}
