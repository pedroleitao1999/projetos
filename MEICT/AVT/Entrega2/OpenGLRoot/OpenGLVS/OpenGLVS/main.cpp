///////////////////////////////////////////////////////////////////////////////
//
// Assignment 1 consists in the following:
//
// - Update your graphics drivers to their latest versions.
// - Download the appropriate libraries GLEW and GLFW for your system.
// - Create a project to compile, link and run the code provided in this 
//   section in your favourite programming environment 
//   (course will use VS2019 Community Edition).
// - Verify what OpenGL contexts your computer can support, a minimum of 
//   OpenGL 3.3 support is required for this course.
//
// Further suggestions to verify your understanding of the concepts explored:
// - Create an abstract class for an OpenGL application.
// - Change the program so display is called at 30 FPS.
//
// (c)2013-20 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vector.h"
#include "matrix.h"

using namespace std;

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	// Challenge 1
	Vector3d v = Vector3d(0.8, 2.7, 1.8);
	Vector3d a = Vector3d(1, 2, 3);
	double angle = 40;
	Vector3d res = Matrix3x3::RRF(v, a, angle);
	cout << res.toString() << "\n" << endl;


	v = Vector3d(0.1, 0.9, 1.5);
	angle = 0;
	res = Matrix3x3::RRF(v, a, angle);
	cout << res.toString() << "\n" << endl;


	a = Vector3d(0, 0, 0);
	res = Matrix3x3::RRF(v, a, angle);
	cout << res.toString() << "\n" << endl;


	// Challenge 2
	Matrix3x3 m = Matrix3x3(5, 1, 1,
		1, 5, 1,
		1, 1, 5);

	Matrix3x3 n = Matrix3x3(1, 2, 3,
		4, 5, 6,
		7, 8, 9);

	Matrix3x3 m_t = m.transpose();
	Matrix3x3 n_t = n.transpose();

	cout << "M transposed: \n" << m_t.Matrix3x3::toString() << endl;
	cout << "N transposed: \n" << n_t.Matrix3x3::toString() << "\n" << endl;

	double m_det = m.determinant();
	double n_det = n.determinant();
	
	cout << "M determinant: \n" << m_det << endl;
	cout << "N determinant: \n" << n_det << "\n" << endl;

	Matrix3x3 m_inv = m.inverse();
	Matrix3x3 n_inv = n.inverse();

	cout << "M inversed: \n" << m_inv.Matrix3x3::toString() << endl;
	cout << "N inversed: \n" << n_inv.Matrix3x3::toString() << "\n" << endl;

	// Challenge 3
	Matrix4x4 trans = Matrix4x4::translation(1, 2, 3);
	Matrix4x4 scale = Matrix4x4::scale(1, 2, 3);
	Matrix4x4 rotY = Matrix4x4::rotationY(12);

	cout << "Translation matrix: \n" << trans.Matrix4x4::toString() << endl;
	cout << "Scale matrix: \n" << scale.Matrix4x4::toString() << endl;
	cout << "Roation matrix: \n" << rotY.Matrix4x4::toString() << "\n" << endl;

	exit(EXIT_SUCCESS);

};

/////////////////////////////////////////////////////////////////////////// END