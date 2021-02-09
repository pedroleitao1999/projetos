#include "../HeaderFiles/Vec3D.h"
#include "../HeaderFiles/Matrix3D.h"
#include "../HeaderFiles/Matrix4D.h"
#include "../HeaderFiles/Matrix2D.h"

Matrix3D::Matrix3D(vector<Vec3D> v) {
    SetMatrix(v);
}

Matrix3D::Matrix3D(vector<float> f) {
    SetMatrix(f);
}

void Matrix3D::SetMatrix(vector<Vec3D> v) {
    if (v.size() != 3) throw invalid_argument("Invalid number of rows in 3x3 matrix");
    vectors = vector<Vec3D>(v);
}

void Matrix3D::SetMatrix(vector<float> f) {
    if (f.size() != 9) throw invalid_argument("Invalid number of values in 3x3 matrix");
    Vec3D row1(f[0], f[1], f[2]);
    Vec3D row2(f[3], f[4], f[5]);
    Vec3D row3(f[6], f[7], f[8]);
    SetMatrix({row1, row2, row3});
}

Matrix3D Matrix3D::identity() {
    return Matrix3D({1,0,0,
                     0,1,0,
                     0,0,1});
}

Vec3D Matrix3D::getVec(int i) {
    return vectors[i];
}

Vec3D Matrix3D::operator[](int i) {
    return getVec(i);
}

Matrix3D Matrix3D::add(Matrix3D b) {
    return Matrix3D( {(*this)[0] + b[0], (*this)[1] + b[1], (*this)[2] + b[2]} );
}

Matrix3D Matrix3D::operator+(Matrix3D b) {
	return add(b);
}

Matrix3D Matrix3D::sub(Matrix3D b) {
    return Matrix3D( {(*this)[0] - b[0], (*this)[1] - b[1], (*this)[2] - b[2]} );
}

Matrix3D Matrix3D::operator-(Matrix3D b) {
	return sub(b);
}

Matrix3D Matrix3D::operator*(float b) {
	return mul(b);
}

Matrix3D Matrix3D::mul(float b) {
    return Matrix3D( {(*this)[0] * b, (*this)[1] * b, (*this)[2] * b} );
}

Vec3D Matrix3D::operator*(Vec3D b) {
	return mul(b);
}

Vec3D Matrix3D::mul(Vec3D b) {
    return Vec3D( (*this)[0][0] * b[0] + (*this)[0][1] * b[1] + (*this)[0][2] * b[2],
                  (*this)[1][0] * b[0] + (*this)[1][1] * b[1] + (*this)[1][2] * b[2],
                  (*this)[2][0] * b[0] + (*this)[2][1] * b[1] + (*this)[2][2] * b[2]);
}

Matrix3D Matrix3D::operator*(Matrix3D b) {
	return mul(b);
}

Matrix3D Matrix3D::mul(Matrix3D b) {
    Vec3D a1( (*this)[0][0], (*this)[0][1], (*this)[0][2]); // row 1 vector of matrix a
    Vec3D a2( (*this)[1][0], (*this)[1][1], (*this)[1][2]); // row 2 vector of matrix a
    Vec3D a3( (*this)[2][0], (*this)[2][1], (*this)[2][2]); // row 3 vector of matrix a
    Vec3D b1( b[0][0], b[1][0], b[2][0]); // col 1 vector of matrix b
    Vec3D b2( b[0][1], b[1][1], b[2][1]); // col 2 vector of matrix b
    Vec3D b3( b[0][2], b[1][2], b[2][2]); // col 3 vector of matrix b

    return Matrix3D( {a1.dot(b1), a1.dot(b2), a1.dot(b3),
                      a2.dot(b1), a2.dot(b2), a2.dot(b3),
                      a3.dot(b1), a3.dot(b2), a3.dot(b3)} );
}

bool Matrix3D::operator==(Matrix3D b) {
    return (*this)[0] == b[0] && (*this)[1] == b[1] && (*this)[2] == b[2];
}

bool Matrix3D::operator!=(Matrix3D b) {
    return (*this)[0] != b[0] || (*this)[1] != b[1] || (*this)[2] != b[2];
}

Matrix3D Matrix3D::transpose() {
    return Matrix3D( {(*this)[0][0], (*this)[1][0], (*this)[2][0],
                      (*this)[0][1], (*this)[1][1], (*this)[2][1],
                      (*this)[0][2], (*this)[1][2], (*this)[2][2]} );
}

float Matrix3D::det() {
	return (*this)[0][0] * Matrix2D({(*this)[1][1], (*this)[1][2], (*this)[2][1], (*this)[2][2]}).det() -
            (*this)[0][1] * Matrix2D({(*this)[1][0], (*this)[1][2], (*this)[2][0], (*this)[2][2]}).det() +
            (*this)[0][2] * Matrix2D({(*this)[1][0], (*this)[1][1], (*this)[2][0], (*this)[2][1]}).det();
}


Matrix3D Matrix3D::inverse() {
    float determinant = det();
	if(determinant != 0) {
		float scalar = 1 / determinant;
		return (Matrix3D({ Matrix2D({(*this)[1][1], (*this)[1][2], (*this)[2][1], (*this)[2][2]}).det(),
                          Matrix2D({(*this)[0][2], (*this)[0][1], (*this)[2][2], (*this)[2][1]}).det(),
                          Matrix2D({(*this)[0][1], (*this)[0][2], (*this)[1][1], (*this)[1][2]}).det(),
                          Matrix2D({(*this)[1][2], (*this)[1][0], (*this)[2][2], (*this)[2][0]}).det(),
                          Matrix2D({(*this)[0][0], (*this)[0][2], (*this)[2][0], (*this)[2][2]}).det(),
                          Matrix2D({(*this)[0][2], (*this)[0][0], (*this)[1][2], (*this)[1][0]}).det(),
                          Matrix2D({(*this)[1][0], (*this)[1][1], (*this)[2][0], (*this)[2][1]}).det(),
                          Matrix2D({(*this)[0][1], (*this)[0][0], (*this)[2][1], (*this)[2][0]}).det(),
                          Matrix2D({(*this)[0][0], (*this)[0][1], (*this)[1][0], (*this)[1][1]}).det()}) * scalar);
	}
	else {
        cerr << "Invalid matrix to be inverted, determinant is zero. Printing the original matrix instead" << endl;
        return *this;
    }
}

std::ostream& operator<<(std::ostream& a, Matrix3D b) {
	Vec3D first = b[0];
	Vec3D second = b[1];
	Vec3D third = b[2];
	return a << "|" << first.getX() << " " << first.getY() << " " << first.getZ() << "|\n" <<
	"|" << second.getX() << " " << second.getY() << " " << second.getZ() << "|\n" <<
	"|" << third.getX() << " " << third.getY() << " " << third.getZ() << "|";
}

Matrix3D operator*(float a, Matrix3D b) {
	return b.mul(a);
}

GLfloat* Matrix3D::toGLfloats() {
    GLfloat* gl = new GLfloat[9] { (*this)[0][0], (*this)[1][0], (*this)[2][0],
                       (*this)[0][1], (*this)[1][1], (*this)[2][1],
                       (*this)[0][2], (*this)[1][2], (*this)[2][2] };
    return gl;
}


Matrix4D Matrix3D::convertTo4D() {
    return (Matrix4D({ (*this)[0][0], (*this)[0][1], (*this)[0][2], 0,
                       (*this)[1][0], (*this)[1][1], (*this)[1][2], 0,
                       (*this)[2][0], (*this)[2][1], (*this)[2][2], 1,
                        0,             0,             0,            1}));
}


Matrix4D Matrix3D::convertToView(Vec3D e) {
    return (Matrix4D({ (*this)[0][0], (*this)[0][1], (*this)[0][2], e.getX(),
                       (*this)[1][0], (*this)[1][1], (*this)[1][2], e.getY(),
                       (*this)[2][0], (*this)[2][1], (*this)[2][2], e.getZ(),
                        0,             0,             0,            1 }));
}