 ///////////////////////////////////////////////////////////////////////
//
// P3D Course
// (c) 2021 by João Madeiras Pereira
//Ray Tracing P3F scenes and drawing points with Modern OpenGL
//
///////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <conio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>

#include "scene.h"
#include "maths.h"
#include "sampler.h"
#include "rayAccelerator.h"

#define CAPTION "Whitted Ray-Tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1

#define MAX_DEPTH 6

unsigned int FrameCount = 0;

// Current Camera Position
float camX, camY, camZ;

//Original Camera position;
Vector Eye;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 0.0f, beta = 0.0f;
float r = 4.0f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];

//Enable OpenGL drawing.  
bool drawModeEnabled = true;

bool P3F_scene = true; //choose between P3F scene or a built-in random scene

// Points defined by 2 attributes: positions which are stored in vertices array and colors which are stored in colors array
float *colors;
float *vertices;
int size_vertices;
int size_colors;

//Array of Pixels to be stored in a file by using DevIL library
uint8_t *img_Data;

GLfloat m[16];  //projection matrix initialized by ortho function

GLuint VaoId;
GLuint VboId[2];

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

Scene* scene = NULL;
int RES_X, RES_Y;

enum Accelerator {NONE, GRID_ACC, BVH_ACC};
Accelerator accel_struct = GRID_ACC;
Grid* grid_ptr;
BVH* bvh_ptr;

int WindowHandle = 0;

float refractionFun(Vector normal, Color& refraction, float ior_1, Ray ray, Vector hitpoint, int numberOfObject, float depth, int currentPixel);

bool antiAliasing = false;
bool softShadows = false;
bool dof = false;
bool fuzzy = false;

int n = 2;
int jittering[4];

Vector getRandomInSphere(Vector center, double radius) {
	double u = rand_double(0, 1);
	double v = rand_double(0, 1);

	// random spherical coordinates, uniformly distributed over sphere
	double theta = 2 * PI * u ;
	double phi = acos(2 * v - 1);

	double x = center.x + (radius * sin(phi) * cos(theta));
	double y = center.y + (radius * sin(phi) * cos(theta));
	double z = center.z + (radius * sin(phi));

	return Vector(x, y, z);
}
//Blinn-Phong model illumination
Color calculateColor(int j, int numberOfObject, Vector normal, Vector L, Vector h) {
	Color lightColor = scene->getLight(j)->color;
	Color diffColor = scene->getObject(numberOfObject)->GetMaterial()->GetDiffColor() * scene->getObject(numberOfObject)->GetMaterial()->GetDiffuse();
	Color specColor = scene->getObject(numberOfObject)->GetMaterial()->GetSpecColor() * scene->getObject(numberOfObject)->GetMaterial()->GetSpecular();

	float dotNL = (max(normal * L, 0));
	float shininess = scene->getObject(numberOfObject)->GetMaterial()->GetShine();
	float spec = (std::pow(max(h * normal, 0), shininess));

	return diffColor* lightColor* dotNL + specColor * lightColor * spec;
}
//check closest intersection
bool trace(Ray ray, Object* &hitOBJ, float& t, int& numberOfObject) {
	hitOBJ = NULL;
	for (int i = 0; i < scene->getNumObjects(); i++) {
		float mint = FLT_MAX;
		if (scene->getObject(i)->intercepts(ray, mint) && mint < t) {
			hitOBJ = scene->getObject(i);
			t = mint;
			numberOfObject = i;
		}
	}
	return (hitOBJ != NULL);
}

Color rayTracing(Ray ray, int depth, float ior_1, int currentPixel)  //index of refraction of medium 1 where the ray is travelling
{
	Color color = Color(0, 0, 0);
	Color reflection, refraction;
	float t = FLT_MAX;
	int numberOfObject = 0;
	int numberOfObject2 = 0;
	float Kr=1; //reflection coefficient
	bool flagHitObject = false;

	Object* hitOBJ = NULL;

	Vector hitpoint = Vector();
	Vector normal = Vector();

	//checking which acceleration structure is active and calls closest intersection functions
	if (accel_struct == GRID_ACC) {
		if (grid_ptr->Traverse(ray, &hitOBJ, hitpoint)) {
			numberOfObject = hitOBJ->getNumber();
			flagHitObject = true;
		}
	}
	else if (accel_struct == BVH_ACC) {
		if (bvh_ptr->Traverse(ray, &hitOBJ, hitpoint)) {
			numberOfObject = hitOBJ->getNumber();
			flagHitObject = true;
		}
	}
	else {
		if (trace(ray, hitOBJ, t, numberOfObject)) {
			hitpoint = ray.origin + ray.direction * t;
			flagHitObject = true;
		}
	}

	if (!flagHitObject) return scene->GetBackgroundColor();
	else {
		normal = scene->getObject(numberOfObject)->getNormal(hitpoint);
		Vector r;

		for (int j = 0; j < scene->getNumLights(); j++) {
			//softshadows
			if (softShadows) {
				//with Anti-aliasing
				if (antiAliasing) {
					int row = currentPixel % n;
					int column = currentPixel;
					while (column >= n) {
						column -= n;
					}
					r = scene->getLight(j)->position + (Vector(0.3 * ((row + rand_float()) / n), 0, 0)) + (Vector(0, 0.3 * ((column + rand_float()) / n), 0));
				}
				//without Anti-aliasing
				else {
					for (int test = 0; test < n; test++) {
						for (int test2 = 0; test2 < n; test2++) {
							r = scene->getLight(j)->position + Vector(0.3 * ((test2 + 1) / n), 0, 0) + Vector(0, 0.3 * ((test + 1) / n), 0);

							Vector L = (r - hitpoint);
							float length = L.length();
							L.normalize();
							float tNearShadow = FLT_MAX;

							bool isInShadow;
							if (accel_struct == GRID_ACC) {
								Vector Ls = (r - hitpoint);
								float length = Ls.length();
								isInShadow = grid_ptr->Traverse(Ray(hitpoint + normal * 0.0001, Ls));
							}
							else if (accel_struct == BVH_ACC) {
								Vector Ls = (r - hitpoint);
								float length = Ls.length();
								isInShadow = bvh_ptr->Traverse(Ray(hitpoint + normal * 0.0001, Ls));
							}
							else {
								isInShadow = (trace(Ray(hitpoint + normal * 0.001, L), hitOBJ, tNearShadow, numberOfObject2));
							}

							Vector h = (L - ray.direction).normalize();
							if (L * normal > 0) {
								if (!isInShadow) {
									Color test = calculateColor(j, numberOfObject, normal, L, h) / (n * n);
									color += test;
								}
							}
						}
					}
				}
			}
			//no soft shadows
			else {
				r = scene->getLight(j)->position;
			}
			if (!(softShadows && !antiAliasing)) {
				
				Vector L = (r - hitpoint);
				float length = L.length();
				L.normalize();

				float tNearShadow = FLT_MAX;
				//calculates shadows
				bool isInShadow;
				if (accel_struct == GRID_ACC) {
					Vector Ls = (r - hitpoint);
					float length = Ls.length();
					isInShadow = grid_ptr->Traverse(Ray(hitpoint + normal * 0.0001, Ls));
				}
				else if (accel_struct == BVH_ACC) {
					Vector Ls = (r - hitpoint);
					float length = Ls.length();
					isInShadow = bvh_ptr->Traverse(Ray(hitpoint + normal * 0.0001, Ls));
				}
				else {
					isInShadow = (trace(Ray(hitpoint + normal * 0.001, L), hitOBJ, tNearShadow, numberOfObject2));
				}

				Vector h = (L - ray.direction).normalize();
				//calls Blinn-Phong calculation function
				if (L * normal > 0) {
					if (!isInShadow) {
						color += calculateColor(j, numberOfObject, normal, L, h);
					}
				}
			}
		}
	}

	if (depth >= MAX_DEPTH) return color.clamp();
	//reflection
	if (scene->getObject(numberOfObject)->GetMaterial()->GetReflection() > 0) {
		Vector reflectionDir = ray.direction - normal * 2 * (ray.direction * normal);;
		double roughness = 0.6;
		//fuzzy reflection
		if (fuzzy) {
			Vector center = (hitpoint + normal * 0.001) + reflectionDir;
			Vector fuzzyDir = (reflectionDir + getRandomInSphere(center, roughness) * roughness).normalize();
			if (normal * fuzzyDir > 0)reflection = rayTracing(Ray(hitpoint + normal * 0.001, fuzzyDir), depth + 1, ior_1, currentPixel);
		}
		//normal reflection
		else {
			reflection = rayTracing(Ray(hitpoint + normal * 0.001, reflectionDir), depth + 1, ior_1, currentPixel);
		}
	}
	//calls refraction function
	if (scene->getObject(numberOfObject)->GetMaterial()->GetTransmittance() != 0) {
		Kr = refractionFun(normal, refraction, ior_1, ray, hitpoint, numberOfObject, depth, currentPixel);
	}
	else {
		Kr = scene->getObject(numberOfObject)->GetMaterial()->GetSpecular();
	}


	Color specColor = scene->getObject(numberOfObject)->GetMaterial()->GetSpecColor();
	color += reflection * Kr * specColor + refraction * (1 - Kr);

	return color.clamp();

}
//calculares refraction Schlick approximation of Fresnel Equations
float refractionFun(Vector normal, Color& refraction, float ior_1, Ray ray, Vector hitpoint, int numberOfObject, float depth, int currentPixel) {
	float nt, ni;
	Vector normalRef = normal;
	Vector InvertedDirection = ray.direction * (-1);
	float cosi = normalRef * InvertedDirection;
	if (cosi > 0) {
		nt = scene->getObject(numberOfObject)->GetMaterial()->GetRefrIndex();
		ni = ior_1 / nt;
	}
	else {
		nt = 1;
		ni = ior_1 / nt;
		normalRef = normal * (-1);
	}

	Vector vt = (normalRef * (cosi)) + ray.direction;
	float sini = vt.length();
	Vector t = vt.normalize();
	float sint = ni * sini;  //snell law
	float costSqrt = sint * sint;

	float R0 = 1;

	if (costSqrt < 1) {
		float cost = sqrt(1 - costSqrt);

		Vector refractionDir = t * sint + normalRef * (-1) * cost;
		refraction = rayTracing(Ray(hitpoint - (normalRef * 0.001), refractionDir.normalize()), depth + 1, nt, currentPixel);

		R0 = pow((ni - nt) / (ni + nt), 2);
	}
	return (R0 + (1 - R0) * pow((1 - cosi), 5));
}



/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

const GLchar* VertexShader =
{
	"#version 430 core\n"

	"in vec2 in_Position;\n"
	"in vec3 in_Color;\n"
	"uniform mat4 Matrix;\n"
	"out vec4 color;\n"

	"void main(void)\n"
	"{\n"
	"	vec4 position = vec4(in_Position, 0.0, 1.0);\n"
	"	color = vec4(in_Color, 1.0);\n"
	"	gl_Position = Matrix * position;\n"

	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 430 core\n"

	"in vec4 color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = color;\n"
	"}\n"
};

void createShaderProgram()
{
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
	glBindAttribLocation(ProgramId, COLOR_ATTRIB, "in_Color");
	
	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs


void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	glGenBuffers(2, VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

	/* Só se faz a alocação dos arrays glBufferData (NULL), e o envio dos pontos para a placa gráfica
	é feito na drawPoints com GlBufferSubData em tempo de execução pois os arrays são GL_DYNAMIC_DRAW */
	glBufferData(GL_ARRAY_BUFFER, size_vertices, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VERTEX_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, size_colors, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(COLOR_ATTRIB);
	glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
	
// unbind the VAO
	glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB); 
//	glDisableVertexAttribArray(COLOR_ATTRIB);
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(COLOR_ATTRIB);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void drawPoints()
{
	FrameCount++;

	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_vertices, vertices);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size_colors, colors);

	glUniformMatrix4fv(UniformId, 1, GL_FALSE, m);
	glDrawArrays(GL_POINTS, 0, RES_X*RES_Y);
	glFinish();

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

ILuint saveImgFile(const char *filename) {
	ILuint ImageId;

	ilEnable(IL_FILE_OVERWRITE);
	ilGenImages(1, &ImageId);
	ilBindImage(ImageId);

	ilTexImage(RES_X, RES_Y, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, img_Data /*Texture*/);
	ilSaveImage(filename);

	ilDisable(IL_FILE_OVERWRITE);
	ilDeleteImages(1, &ImageId);
	if (ilGetError() != IL_NO_ERROR)return ilGetError();

	return IL_NO_ERROR;
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << RES_X << "x" << RES_Y << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutTimerFunc(1000, timer, 0);
}


// Render function by primary ray casting from the eye towards the scene's objects

void renderScene()
{
	int index_pos=0;
	int index_col=0;
	unsigned int counter = 0;

	if (drawModeEnabled) {
		glClear(GL_COLOR_BUFFER_BIT);
		scene->GetCamera()->SetEye(Vector(camX, camY, camZ));  //Camera motion
	}
	
	Vector unit_disk;
	Ray ray = Ray(Vector(0,0,0), Vector(0,0,0));

	for (int i = 0; i < (n * n); i++) {
		jittering[i] = i;
	}
	
	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{
			Color color = Color(0,0,0); 
			//shuffle array 
			for (int i = (n*n) - 1; i >= 1; i--){
				int j = rand() % (i + 1);

				int temp = jittering[i];
				jittering[i] = jittering[j];
				jittering[j] = temp;
			}
			int current = 0;
			Vector pixel;  //viewport coordinates
			if (antiAliasing) {
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < n; j++) {
						//jittering
						pixel.x = x + (i + rand_float()) / n;
						pixel.y = y + (j + rand_float()) / n;
						if (dof) {
							//depth of field
							unit_disk = sample_unit_disk();
							ray = scene->GetCamera()->PrimaryRay(unit_disk, pixel);
						}
						else {
							ray = scene->GetCamera()->PrimaryRay(pixel);
						}
						color = color + rayTracing(ray, 1, 1.0, jittering[current]).clamp();
						current++;
					}
				}
				color.r(color.r() / (n * n));
				color.g(color.g() / (n * n));
				color.b(color.b() / (n * n));
			}
			else {
				//no anti aliasing
				pixel.x = x;
				pixel.y = y;

				Ray ray = scene->GetCamera()->PrimaryRay(pixel);

				color = color + rayTracing(ray, 1, 1.0,0).clamp();
			}


			img_Data[counter++] = u8fromfloat((float)color.r());
			img_Data[counter++] = u8fromfloat((float)color.g());
			img_Data[counter++] = u8fromfloat((float)color.b());

			if (drawModeEnabled) {
				vertices[index_pos++] = (float)x;
				vertices[index_pos++] = (float)y;
				colors[index_col++] = (float)color.r();

				colors[index_col++] = (float)color.g();

				colors[index_col++] = (float)color.b();
			}
		}
	
	}
	if(drawModeEnabled) {
		drawPoints();
		glutSwapBuffers();
	}
	else {
		printf("Terminou o desenho!\n");
		if (saveImgFile("RT_Output.png") != IL_NO_ERROR) {
			printf("Error saving Image file\n");
			exit(0);
		}
		printf("Image file created\n");
	}
}

// Callback function for glutCloseFunc
void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void ortho(float left, float right, float bottom, float top, 
			float nearp, float farp)
{
	m[0 * 4 + 0] = 2 / (right - left);
	m[0 * 4 + 1] = 0.0;
	m[0 * 4 + 2] = 0.0;
	m[0 * 4 + 3] = 0.0;
	m[1 * 4 + 0] = 0.0;
	m[1 * 4 + 1] = 2 / (top - bottom);
	m[1 * 4 + 2] = 0.0;
	m[1 * 4 + 3] = 0.0;
	m[2 * 4 + 0] = 0.0;
	m[2 * 4 + 1] = 0.0;
	m[2 * 4 + 2] = -2 / (farp - nearp);
	m[2 * 4 + 3] = 0.0;
	m[3 * 4 + 0] = -(right + left) / (right - left);
	m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	m[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
	m[3 * 4 + 3] = 1.0;
}

void reshape(int w, int h)
{
    glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	ortho(0, (float)RES_X, 0, (float)RES_Y, -1.0, 1.0);
}
//changes values of flags
void processKeys(unsigned char key, int xx, int yy)
{
	switch (key) {

		case 27:
			glutLeaveMainLoop();
			break;

		case 'r':
			camX = Eye.x;
			camY = Eye.y;
			camZ = Eye.z;
			r = Eye.length();
			beta = asinf(camY / r) * 180.0f / 3.14f;
			alpha = atanf(camX / camZ) * 180.0f / 3.14f;
			break;

		case 'c':
			printf("Camera Spherical Coordinates (%f, %f, %f)\n", r, beta, alpha);
			printf("Camera Cartesian Coordinates (%f, %f, %f)\n", camX, camY, camZ);
			break;

		case 'a':
			antiAliasing = !antiAliasing;
			break;

		case 's':
			softShadows = !softShadows;
			break;

		case 'd':
			dof = !dof;
			break;

		case 'f':
			fuzzy = !fuzzy;
			break;
	}
}


// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux = 0.0, betaAux = 0.0;
	float rAux = 0.0;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux * sin(betaAux * 3.14f / 180.0f);
}

void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r * sin(beta * 3.14f / 180.0f);
}


/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutKeyboardFunc(processKeys);
	glutCloseFunc(cleanup);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc(mouseWheel);

	glutIdleFunc(renderScene);
	glutTimerFunc(0, timer, 0);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	GLenum err_code = glGetError();
	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(4, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowPosition(100, 250);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glDisable(GL_DEPTH_TEST);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}


void init(int argc, char* argv[])
{
	// set the initial camera position on its spherical coordinates
	Eye = scene->GetCamera()->GetEye();
	camX = Eye.x;
	camY = Eye.y;
	camZ = Eye.z;
	r = Eye.length();
	beta = asinf(camY / r) * 180.0f / 3.14f;
	alpha = atanf(camX / camZ) * 180.0f / 3.14f;

	setupGLUT(argc, argv);
	setupGLEW();
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
	
}


void init_scene(void)
{
	char scenes_dir[70] = "P3D_Scenes/";
	char input_user[50];
	char scene_name[70];

	scene = new Scene();

	if (P3F_scene) {  //Loading a P3F scene

		while (true) {
			cout << "Input the Scene Name: ";
			cin >> input_user;
			strcpy_s(scene_name, sizeof(scene_name), scenes_dir);
			strcat_s(scene_name, sizeof(scene_name), input_user);

			ifstream file(scene_name, ios::in);
			if (file.fail()) {
				printf("\nError opening P3F file.\n");
			}
			else
				break;
		}

		scene->load_p3f(scene_name);
	}
	else {
		printf("Creating a Random Scene.\n\n");
		scene->create_random_scene();
	}
	RES_X = scene->GetCamera()->GetResX();
	RES_Y = scene->GetCamera()->GetResY();
	printf("\nResolutionX = %d  ResolutionY= %d.\n", RES_X, RES_Y);

	
	vector<Object*> objs;
	int num_objects = scene->getNumObjects();
	for (int o = 0; o < num_objects; o++) {
		objs.push_back(scene->getObject(o));
	}

	if (accel_struct == GRID_ACC) {
		grid_ptr = new Grid();
		grid_ptr->Build(objs);
		printf("Grid built.\n\n");

	}
	
	else if (accel_struct == BVH_ACC) {
		bvh_ptr = new BVH();
		bvh_ptr->Build(objs);
		printf("BVH built.\n\n");
	}
	

	// Pixel buffer to be used in the Save Image function
	img_Data = (uint8_t*)malloc(3 * RES_X*RES_Y * sizeof(uint8_t));
	if (img_Data == NULL) exit(1);
}

int main(int argc, char* argv[])
{
	//Initialization of DevIL 
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		printf("wrong DevIL version \n");
		exit(0);
	}
	ilInit();

	int ch;
	if (!drawModeEnabled) {

		do {
			init_scene();
			auto timeStart = std::chrono::high_resolution_clock::now();
			renderScene();  //Just creating an image file
			auto timeEnd = std::chrono::high_resolution_clock::now();
			auto passedTime = std::chrono::duration<double, std::milli>(timeEnd - timeStart).count();
			printf("\nDone: %.2f (sec)\n", passedTime / 1000);
			if (!P3F_scene) break;
			cout << "\nPress 'y' to render another image or another key to terminate!\n";
			delete(scene);
			free(img_Data);
			ch = _getch();
		} while((toupper(ch) == 'Y')) ;
	}

	else {   //Use OpenGL to draw image in the screen
		printf("OPENGL DRAWING MODE\n\n");
		init_scene();
		size_vertices = 2 * RES_X*RES_Y * sizeof(float);
		size_colors = 3 * RES_X*RES_Y * sizeof(float);
		vertices = (float*)malloc(size_vertices);
		if (vertices == NULL) exit(1);
		colors = (float*)malloc(size_colors);
		if (colors == NULL) exit(1);
	   
		/* Setup GLUT and GLEW */
		init(argc, argv);
		glutMainLoop();
	}

	free(colors);
	free(vertices);
	printf("Program ended normally\n");
	exit(EXIT_SUCCESS);
}
///////////////////////////////////////////////////////////////////////