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

using namespace std;

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{

	double quadrupleArray[10];
	for (int i = 0; i < 10; i++) {

		Vector3d arrayVec[4] = {Vector3d(0, 0, 0), Vector3d(0, 0, 0), Vector3d(0, 0, 0), Vector3d(0, 0, 0)};

		for (int j = 0; j < 4; j++) {
			double randX = std::rand() % 20 + (double) (-10);
			double randY = std::rand() % 20 + (double) (-10);
			double randZ = std::rand() % 20 + (double) (-10);
			Vector3d vec = Vector3d(randX, randY, randZ);
			vec = vec.normalize();
			arrayVec[j] = vec;

			// Printing vectors:
			cout << "Normalized vector " << j << ": [" 
			<< arrayVec[j].getX() << ", " 
			<< arrayVec[j].getY() << ", " 
			<< arrayVec[j].getZ() << "]" << endl;
		}

		double result = Vector3d::quadruple(arrayVec[0], arrayVec[1], arrayVec[2], arrayVec[3]);
		quadrupleArray[i] = result;

		// Printing result:
		cout << "Result of quadruple product: " << quadrupleArray[i] << "\n" << endl;
	}
	
	cout << "For the following comparisons, 1 is true and 0 is false: " << endl;
	cout << ( Vector3d(2, 4, 6) == Vector3d(0.1, 0.2, 0.3) * 20 ) << endl;
	cout << ( Vector3d(0.2, 0.3, 0.4) + Vector3d(1, 2, 3) == Vector3d(1.2, 2.3, 3.4) ) << endl;
	cout << ( Vector3d(0.2, 0.3, 0.4) - Vector3d(2, 3, 4) != Vector3d(-0.8, -1.7, -3.59) ) << endl;
	
	exit(EXIT_SUCCESS);

};

/////////////////////////////////////////////////////////////////////////// END