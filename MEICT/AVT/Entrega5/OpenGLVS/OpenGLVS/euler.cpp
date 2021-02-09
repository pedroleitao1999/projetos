#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <math.h> 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "main.h"

///////////////// Euler rotation mode /////////////////

// Rotation arround the x axis
Matrix3x3 Matrix3x3::rotationInX(GLfloat angle) {
	Matrix3x3 Rx = Matrix3x3(
		1.0f, 0.0f, 0.0f,
		0.0f, (GLfloat)cos(angle), (GLfloat)-sin(angle),
		0.0f, (GLfloat)sin(angle), (GLfloat)cos(angle)
	);
	return Rx;
}

// Rotation arround the y axis
Matrix3x3 Matrix3x3::rotationInY(GLfloat angle) {
	Matrix3x3 Ry = Matrix3x3(
		(GLfloat)cos(angle), 0.0f, (GLfloat)sin(angle),
		0.0f, 1.0f, 0.0f,
		(GLfloat)-sin(angle), 0.0f, (GLfloat)cos(angle)
	);
	return Ry;
}

// Rotation arround the z axis
Matrix3x3 Matrix3x3::rotationInZ(GLfloat angle) {
	Matrix3x3 Rz = Matrix3x3(
		(GLfloat)cos(angle), (GLfloat)-sin(angle), 0.0f,
		(GLfloat)sin(angle), (GLfloat)cos(angle), 0.0f,
		0.0f, 0.0f, 1.0f
	);
	return Rz;
}

// Euler rotation using Trait-Bryan angles
Matrix3x3 Matrix3x3::eulerRotation(GLfloat yaw, GLfloat pitch, GLfloat roll) {
	Matrix3x3 EulerRot = rotationInX(roll) * rotationInZ(pitch) * rotationInY(yaw);
	return EulerRot;
}