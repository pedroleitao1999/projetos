#include "../HeaderFiles/Vec3D.h"
#include "../HeaderFiles/Matrix3D.h"
#include "../HeaderFiles/Matrix4D.h"
#include "../HeaderFiles/Camera.h"

Camera::Camera(vector<Vec3D> v) {
	SetCamera(v);
}

Camera::Camera() {}

void Camera::SetCamera(vector <Vec3D> j) {
	if (j.size() != 3) throw invalid_argument("Invalid number of vectors");
	eye = j[0];
	center = j[1];
	up = j[2];
	v = (center - eye).normalize();
	s = (v.cross(up)).normalize();
	u = s.cross(v);
}


Vec3D Camera::getEye() {
	return eye;
}
Vec3D Camera::getCenter() {
	return center;
}
Vec3D Camera::getUp() {
	return up;
}
Vec3D Camera::getS() {
	return s;
}
Vec3D Camera::getV() {
	return v;
}
Vec3D Camera::getU() {
	return u;
}
Vec3D Camera::getNegativeV() {
	return v * (-1);
}

double convertToR(float degree) {
	double pi = 3.14159265359;
	return (degree * (pi / 180));

}

Matrix4D Camera::convertToView(Matrix3D m,Vec3D e) {
	return (Matrix4D({ (m)[0][0], (m)[0][1], (m)[0][2], e.getX(),
					   (m)[1][0], (m)[1][1], (m)[1][2], e.getY(),
					   (m)[2][0], (m)[2][1], (m)[2][2], e.getZ(),
						0,             0,             0,            1 }));
}


Matrix4D Camera::perspectiveP(float fov, float aspect, float zNear, float zFar) {
	float d = 1 / (float)tan(convertToR(fov / 2));
	return Matrix4D({ (d / aspect),        0,                               0,                                  0,
					  0         ,        d,                               0,                                  0,
					  0         ,        0, (zNear + zFar) / (zNear - zFar), (2 * zNear * zFar) / (zNear - zFar),
					  0         ,        0,                              -1,                                 0 });
}

Matrix4D Camera::ortographicP(float left, float right, float bottom, float top, float near, float far) {
	return Matrix4D({ 2 / (right - left),                  0,                0,     (left + right) / (left - right),
					  0                 , 2 / (top - bottom),                0,     (bottom + top) / (bottom - top),
					  0                 ,                  0, 2 / (near - far),         (near + far) / (near - far),
					  0                 ,                  0,                0,                                 1 });
}

