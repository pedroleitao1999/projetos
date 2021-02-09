#include "../HeaderFiles/Quat.h"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

const float qThreshold = (float)1.0e-5;

Quat::Quat(float t, float x, float y, float z) {
	SetQuat(t, x, y, z);
}

float Quat::getT() {
	return t;
}
float Quat::getX() {
	return x;
}
float Quat::getY() {
	return y;
}
float Quat::getZ() {
	return z;
}

void Quat::SetQuat(float a, float b, float c, float d) {
	t = a;
	x = b;
	y = c;
	z = d;
}

Quat Quat::qFromAngleAxis(float theta, Vec4D axis) {
	Vec4D axisn = axis.normalize();
	float a = theta * (float)DEGREES_TO_RADIANS;
	float s = sin(a / 2.0f);
	Quat q(cos(a/2.0f), axisn.getX() * s, axisn.getY() * s, axisn.getZ() * s);
	return q.normalize();
}

void Quat::qToAngleAxis(Quat q, float theta, Vec4D axis) {
	Quat qn = q.normalize();
	theta = 2.0f * acos(qn.getT()) * (float)RADIANS_TO_DEGREES ;
	float s = sqrt(1.0f - qn.getT() * qn.getT());
	if (s < qThreshold) {
		axis.SetVector(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		float sinv = 1 / s;
		axis.SetVector(qn.getX() * sinv,qn.getY()*sinv, qn.getZ() * sinv,1.0f);
	}
}



Quat Quat::normalize() {
	float result = (*this).getT() * (*this).getT() + (*this).getX() * (*this).getX() + (*this).getY() * (*this).getY() + (*this).getZ() * (*this).getZ();
	float s = 1 / sqrt(result);
	return (*this).mul(s);
}

Quat Quat::mul(float s) {
	return Quat((*this).getT() * s, (*this).getX() * s, (*this).getY() * s, (*this).getZ() * s);
}

Quat Quat::mul(Quat q) {
	float q1 = (*this).getT() * q.getT() - (*this).getX() * q.getX() - (*this).getY() * q.getY() - (*this).getZ() * q.getZ();
	float q2 = (*this).getT() * q.getX() + (*this).getX() * q.getT() + (*this).getY() * q.getZ() - (*this).getZ() * q.getY();
	float q3 = (*this).getT() * q.getY() + (*this).getY() * q.getT() + (*this).getZ() * q.getX() - (*this).getX() * q.getZ();
	float q4 = (*this).getT() * q.getZ() + (*this).getZ() * q.getT() + (*this).getX() * q.getY() - (*this).getY() * q.getX();

	return Quat(q1, q2, q3, q4);
}

Quat Quat::add(Quat q) {
	return Quat((*this).getT() * q.getT(), (*this).getX() * q.getX(), (*this).getY() * q.getY(), (*this).getZ() * q.getZ());
}


void Quat::rotationMatrix(Matrix4D &matrix) {
	Quat qn = (*this).normalize();
	float xx = qn.getX() * qn.getX();
	float xy = qn.getX() * qn.getY();
	float xz = qn.getX() * qn.getZ();
	float xt = qn.getX() * qn.getT();
	float yy = qn.getY() * qn.getY();
	float yz = qn.getY() * qn.getZ();
	float yt = qn.getY() * qn.getT();
	float zz = qn.getZ() * qn.getZ();
	float zt = qn.getZ() * qn.getT();

	float matrix0 = 1.0f - 2.0f * (yy + zz);
	float matrix1 = 2.0f * (xy + zt);
	float matrix2 = 2.0f * (xz - yt);
	float matrix3 = 0.0f;

	float matrix4 = 2.0f * (xy - zt);
	float matrix5 = 1.0f - 2.0f * (xx + zz);
	float matrix6 = 2.0f * (yz + xt);
	float matrix7 = 0.0f;

	float matrix8 = 2.0f * (xz + yt);
	float matrix9 = 2.0f * (yz - xt);
	float matrix10 = 1.0f - 2.0f * (xx + yy);
	float matrix11 = 0.0f;

	float matrix12 = 0.0f;
	float matrix13 = 0.0f;
	float matrix14 = 0.0f;
	float matrix15 = 1.0f;

	matrix.SetMatrix({ matrix0, matrix1, matrix2,matrix3,matrix4, matrix5,matrix6,matrix7,matrix8,matrix9,matrix10,matrix11,matrix12,matrix13,matrix14,matrix15 });
}	

Quat Quat::lerp(Quat q, float k) {
	float cos_angle = (*this).getX() * q.getX() + (*this).getY() * q.getY() + (*this).getZ() * q.getZ() + (*this).getT() * q.getT();
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	Quat qi = (*this).mul(k0).add(q.mul(k1));
	return qi.normalize();
}

Quat Quat::slerp(Quat q, float k) {
	float angle = acos((*this).getX() * q.getX() + (*this).getY() * q.getY() + (*this).getZ() * q.getZ() + (*this).getT() * q.getT());
	float k0 = sin((1 - k) * angle) / sin(angle);
	float k1 = sin(k * angle) / sin(angle);
	Quat qi = (*this).mul(k0).add(q.mul(k1));
	return qi.normalize();
}