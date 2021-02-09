//
// CAMERA CLASS
//

#include "main.h"

using namespace std;

//////////////// Euler Rotation Mode
GLfloat yaw = (GLfloat)270.0f * DEGREES_TO_RADIANS;
GLfloat pitch = (GLfloat)300.0f * DEGREES_TO_RADIANS;
GLfloat roll = (GLfloat)240.0f * DEGREES_TO_RADIANS;
GLfloat percentage = 0.02f;
GLfloat state = 1.0f;
GLfloat newPercentage = 0.02f;

Matrix4x4 identity = Matrix4x4(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

// Rotation Matrix (120 degrees)
Matrix4x4 R = Matrix4x4(
	-0.5f, 0.866f, 0.0f, -0.4873f,
	-0.866f, -0.5f, 0.0f, 0.8787f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
); // Row Major (GLSL is Column Major)

// Translation Matrix
Matrix4x4 T = Matrix4x4::translation(0.55f, 1.185f, 0.0f);

// Multiplication between Rotation and Translation Matrices
Matrix4x4 MultiplicationRT = R * T;

// Second multiplication between Rotation and Translation Matrices
Matrix4x4 MultiplicationRT2 = MultiplicationRT * MultiplicationRT;

// Matrices for Back Face

// Rotation Matrix (120 degrees)
Matrix4x4 RB = Matrix4x4(
	-0.5f, -0.866f, 0.0f, -0.4873f,
	0.866f, -0.5f, 0.0f, 0.8787f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
); // Row Major (GLSL is Column Major)

// Multiplication between Rotation and Translation Matrices
Matrix4x4 MultiplicationRBT = RB * T;

// Second multiplication between Rotation and Translation Matrices
Matrix4x4 MultiplicationRBT2 = MultiplicationRBT * MultiplicationRBT;

// Initial state for the animated rotation
Matrix4x4 AnimatedRotation = Matrix4x4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

//Camera elements

//View Matrix
Vector3d eyeVec = Vector3d(5.0f, 5.0f, 5.0f);
Vector3d centerVec = Vector3d(0.0f, 0.0f, 0.0f);
Vector3d upVec = Vector3d(0.0f, 1.0f, 0.0f);

//Projection Matrices
GLfloat lP = -2.0f;
GLfloat rP = 2.0f;
GLfloat bt = -2.0f;
GLfloat tp = 2.0f;
GLfloat nr = 1.0f;
GLfloat fr = 10.0f;
GLfloat ang = 30.0f;
GLfloat w = 640.0f;
GLfloat h = 480.0f;

Camera::Camera() {

	//Elements for the View Matrix
	eye = eyeVec;
	center = centerVec;
	up = upVec;

	view = center - eye;
	v = view.normalize();
	side = v * up;
	s = side.normalize();
	s = side.normalize();
	u = s * v;

	//Elements for the Projection Matrices

	leftProj = lP;
	rightProj = rP;
	bottom = bt;
	top = tp;
	near = nr;
	far = fr;
	angle = ang;
	aspect = (GLfloat) w / h;

	tlr = (GLfloat)-(leftProj + rightProj) / 2.0f;
	tbt = (GLfloat)-(bottom + top) / 2.0f;
	tnf = (GLfloat)-(near + far) / 2.0f;

	slr = 2.0f / (rightProj - leftProj);
	sbt = 2.0f / (top - bottom);
	snf = 2.0f / (far - near);

	//Matrices

	identityMatrix = identity;

	viewMatrix = Matrix4x4::createViewMatrix(eye, v, s, u);

	orthographicProjectionMatrix = Matrix4x4::createOrthographicProjectionMatrix(tlr, tbt, tnf, slr, sbt, snf);
	perspectiveProjectionMatrix = Matrix4x4::perspective(angle, aspect, near, far);

};

Matrix4x4 Matrix4x4::generateViewRotation(Vector3d vVec, Vector3d sVec, Vector3d uVec) {
	Matrix4x4 ViewRotation = Matrix4x4(
		sVec.getX(), sVec.getY(), sVec.getZ(), 0.0f,
		uVec.getX(), uVec.getY(), uVec.getZ(), 0.0f,
		-vVec.getX(), -vVec.getY(), -vVec.getZ(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	); // Column Major 
	return ViewRotation;
}

Matrix4x4 Matrix4x4::createViewMatrix(Vector3d eyeVec, Vector3d vVec, Vector3d sVec, Vector3d uVec) {
	//Matrices that build the View Matrix
	Matrix4x4 ViewRotation = Matrix4x4::generateViewRotation(vVec, sVec, uVec);
	Matrix4x4 EyeTranslation = Matrix4x4::translation(-eyeVec.getX(), -eyeVec.getY(), -eyeVec.getZ());
	Matrix4x4 EyeMultiplication = ViewRotation * EyeTranslation;
	return EyeMultiplication;
};

Matrix4x4 Matrix4x4::createOrthographicProjectionMatrix(GLfloat tlrGL, GLfloat tbtGL, GLfloat tnfGL, GLfloat slrGL, GLfloat sbtGL, GLfloat snfGL) {
	//Translation Matrix
	Matrix4x4 OrthographicTranslation = Matrix4x4::translation(tlrGL, tbtGL, tnfGL);

	//Scalar Matrix 
	Matrix4x4 OrthographicScaling = Matrix4x4::scale(slrGL, sbtGL, snfGL);

	//Identity Matrix for the Orthographic Projection Matrix
	Matrix4x4 Identity = Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	// Orthographic Projection Matrix
	Matrix4x4 OrthographicMultiplication = OrthographicScaling * OrthographicTranslation * Identity;
	return OrthographicMultiplication;
};

void Camera::moveCamera(Vector3d newEye) {
	updateEye(newEye);
};

void Camera::orientateCamera(Vector2d delta) {
	GLfloat newX = this->eye.getX() + delta.getX() * 0.01f;
	GLfloat newY = this->eye.getY() + delta.getY() * 0.01f;
	GLfloat newZ = this->eye.getZ();
	Vector3d newEye = Vector3d(newX, newY, newZ);
	updateEye(newEye);
};

void Camera::updateEye(Vector3d newEye) {
	Vector3d oldEye = eye;
	eye = newEye;
	viewMatrix = Matrix4x4::createViewMatrix(newEye, v, s, u);
};