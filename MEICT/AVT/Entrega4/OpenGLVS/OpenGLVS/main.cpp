///////////////////////////////////////////////////////////////////////////////
//
// Adding a camera and moving to the 3rd dimension.
// A "Hello 3D World" of Modern OpenGL.
//
// (c) 2013-20 by Carlos Martinho
//
// Group 12
// 84741 - Marc Alexandre Jelkic
// 90764 - Pedro de Oliveira Rosa Alves Leitao
//
// Debug in x86
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <math.h> 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vector.h"
#include "matrix.h"

#define VERTICES 0
#define COLORS 1

GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UboId, UniformId;
const GLuint UBO_BP = 0;
bool key_pressed  = false;
int width = 640;
int height = 480;
using namespace std;

////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
	std::cerr << "Press <return>.";
	std::cin.ignore();
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
	// params: source, type, severity, count, ids, enabled
}

/////////////////////////////////////////////////////////////////////// SHADERs

const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec4 in_Position;\n"
	"in vec4 in_Color;\n"
	"out vec4 ex_Color;\n"

	"uniform mat4 ModelMatrix;\n"

	"uniform SharedMatrices\n"
	"{\n"
	"	mat4 ViewMatrix;\n"
	"	mat4 ProjectionMatrix;\n"
	"};\n"

	"void main(void)\n"
	"{\n"
	"	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * in_Position;\n"
	"	ex_Color = in_Color;\n"
	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 ex_Color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = ex_Color;\n"
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

	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");
	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "ModelMatrix");
	UboId = glGetUniformBlockIndex(ProgramId, "SharedMatrices");
	glUniformBlockBinding(ProgramId, UboId, UBO_BP);

	glDetachShader(ProgramId, VertexShaderId);
	glDeleteShader(VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(FragmentShaderId);
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);
}

/////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

typedef GLfloat Matrix[16];

const Vertex Vertices[] = 
{

	//Front face

	//Triangle 1
	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }},
	{{ -0.605f, -0.48f, 0.0f, 1.0f }, { 0.3f, 0.0f, 0.0f, 1.0f }},
	{{ -0.54f, -0.605f, 0.0f, 1.0f }, { 0.3f, 0.0f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }},
	{{ -0.54f, -0.605f, 0.0f, 1.0f }, { 0.3f, 0.0f, 0.0f, 1.0f }},
	{{ 0.01f, 0.34f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }},
	{{ 0.01f, 0.34f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }},
	{{ 0.42f, -0.37f, 0.0f, 1.0f }, { 0.3f, 0.0f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f }},
	{{ 0.56f, -0.37f, 0.0f, 1.0f }, { 0.3f, 0.0f, 0.0f, 1.0f }},
	{{ 0.42f, -0.37f, 0.0f, 1.0f }, { 0.3f, 0.0f, 0.0f, 1.0f }},

	//Triangle 2
	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }},
	{{ -0.605f, -0.48f, 0.0f, 1.0f }, { 0.0f, 0.3f, 0.0f, 1.0f }},
	{{ -0.54f, -0.605f, 0.0f, 1.0f }, { 0.0f, 0.3f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }},
	{{ -0.54f, -0.605f, 0.0f, 1.0f }, { 0.0f, 0.3f, 0.0f, 1.0f }},
	{{ 0.01f, 0.34f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }},
	{{ 0.01f, 0.34f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }},
	{{ 0.42f, -0.37f, 0.0f, 1.0f }, { 0.0f, 0.3f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.0f, 0.9f, 0.0f, 1.0f }},
	{{ 0.56f, -0.37f, 0.0f, 1.0f }, { 0.0f, 0.3f, 0.0f, 1.0f }},
	{{ 0.42f, -0.37f, 0.0f, 1.0f }, { 0.0f, 0.3f, 0.0f, 1.0f }},

	//Triangle 3
	{ { 0.01f, 0.58f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }},
	{{ -0.605f, -0.48f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.3f, 1.0f }},
	{{ -0.54f, -0.605f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.3f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }},
	{{ -0.54f, -0.605f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.3f, 1.0f }},
	{{ 0.01f, 0.34f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }},
	{{ 0.01f, 0.34f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }},
	{{ 0.42f, -0.37f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.3f, 1.0f }},

	{{ 0.01f, 0.58f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.9f, 1.0f }},
	{{ 0.56f, -0.37f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.3f, 1.0f }},
	{{ 0.42f, -0.37f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.3f, 1.0f }},

	//Back face

	//Triangle 1
	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.5f, 0.0f, 0.0f, 1.0f }},
	{{ -0.605f, -0.48f, -0.0001f, 1.0f }, { 0.1f, 0.0f, 0.0f, 1.0f }},
	{{ -0.54f, -0.605f, -0.0001f, 1.0f }, { 0.1f, 0.0f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.5f, 0.0f, 0.0f, 1.0f }},
	{{ -0.54f, -0.605f, -0.0001f, 1.0f }, { 0.1f, 0.0f, 0.0f, 1.0f }},
	{{ 0.01f, 0.34f, -0.0001f, 1.0f }, { 0.5f, 0.0f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.5f, 0.0f, 0.0f, 1.0f }},
	{{ 0.01f, 0.34f, -0.0001f, 1.0f }, { 0.5f, 0.0f, 0.0f, 1.0f }},
	{{ 0.42f, -0.37f, -0.001f, 1.0f }, { 0.1f, 0.0f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.5f, 0.0f, 0.0f, 1.0f }},
	{{ 0.56f, -0.37f, -0.0001f, 1.0f }, { 0.1f, 0.0f, 0.0f, 1.0f }},
	{{ 0.42f, -0.37f, -0.0001f, 1.0f }, { 0.1f, 0.0f, 0.0f, 1.0f }},

	//Triangle 2
	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.0f, 0.5f, 0.0f, 1.0f }},
	{{ -0.605f, -0.48f, -0.0001f, 1.0f }, { 0.0f, 0.1f, 0.0f, 1.0f }},
	{{ -0.54f, -0.605f, -0.0001f, 1.0f }, { 0.0f, 0.1f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.0f, 0.5f, 0.0f, 1.0f }},
	{{ -0.54f, -0.605f, -0.0001f, 1.0f }, { 0.0f, 0.1f, 0.0f, 1.0f }},
	{{ 0.01f, 0.34f, -0.0001f, 1.0f }, { 0.0f, 0.1f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.0f, 0.5f, 0.0f, 1.0f }},
	{{ 0.01f, 0.34f, -0.0001f, 1.0f }, { 0.0f, 0.1f, 0.0f, 1.0f }},
	{{ 0.42f, -0.37f, -0.0001f, 1.0f }, { 0.0f, 0.1f, 0.0f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.0f, 0.5f, 0.0f, 1.0f }},
	{{ 0.56f, -0.37f, -0.0001f, 1.0f }, { 0.0f, 0.1f, 0.0f, 1.0f }},
	{{ 0.42f, -0.37f, -0.0001f, 1.0f }, { 0.0f, 0.1f, 0.0f, 1.0f }},

	//Triangle 3
	{ { 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f }},
	{{ -0.605f, -0.48f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.1f, 1.0f }},
	{{ -0.54f, -0.605f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.1f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f }},
	{{ -0.54f, -0.605f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.1f, 1.0f }},
	{{ 0.01f, 0.34f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f }},
	{{ 0.01f, 0.34f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f }},
	{{ 0.42f, -0.37f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.1f, 1.0f }},

	{{ 0.01f, 0.58f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.5f, 1.0f }},
	{{ 0.56f, -0.37f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.1f, 1.0f }},
	{{ 0.42f, -0.37f, -0.0001f, 1.0f }, { 0.0f, 0.0f, 0.1f, 1.0f }}

};

const GLubyte Indices[] =
{
	// Front Face
	0,1,2,
	3,4,5,
	6,7,8,
	9,10,11,
	12,13,14,
	15,16,17,
	18,19,20,
	21,22,23,
	24,25,26,
	27,28,29,
	30,31,32,
	33,34,35,

	// Back Face
	36,37,38,
	39,40,41,
	42,43,44,
	45,46,47,
	48,49,50,
	51,52,53,
	54,55,56,
	57,58,59,
	60,61,62,
	63,64,65,
	66,67,68,
	69,70,71
};

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertices[0].XYZW));
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
	{
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId[1]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////// SCENE


const Matrix I = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
};

// Rotation Matrix (120 degrees)
Matrix4x4 R = Matrix4x4(
	-0.5f,  0.866f,  0.0f, -0.4873f,
	-0.866f,  -0.5f,  0.0f, 0.8787f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
); // Row Major (GLSL is Column Major)

// Translation Matrix
Matrix4x4 T = Matrix4x4::translation(0.55f, 1.185f, 0.0f);

// Multiplication between Rotation and Translation Matrices
Matrix4x4 MultiplicationRT = R * T;
const Matrix RT = {
	MultiplicationRT.getUx(), MultiplicationRT.getUy(), MultiplicationRT.getUz(), MultiplicationRT.getUn(),
	MultiplicationRT.getVx(), MultiplicationRT.getVy(), MultiplicationRT.getVz(), MultiplicationRT.getVn(),
	MultiplicationRT.getWx(), MultiplicationRT.getWy(), MultiplicationRT.getWz(), MultiplicationRT.getWn(),
	MultiplicationRT.getMx(), MultiplicationRT.getMy(), MultiplicationRT.getMz(), MultiplicationRT.getMn()
};

// Second multiplication between Rotation and Translation Matrices
Matrix4x4 MultiplicationRT2 = MultiplicationRT * MultiplicationRT;
const Matrix RT2 = {
	MultiplicationRT2.getUx(), MultiplicationRT2.getUy(), MultiplicationRT2.getUz(), MultiplicationRT2.getUn(),
	MultiplicationRT2.getVx(), MultiplicationRT2.getVy(), MultiplicationRT2.getVz(), MultiplicationRT2.getVn(),
	MultiplicationRT2.getWx(), MultiplicationRT2.getWy(), MultiplicationRT2.getWz(), MultiplicationRT2.getWn(),
	MultiplicationRT2.getMx(), MultiplicationRT2.getMy(), MultiplicationRT2.getMz(), MultiplicationRT2.getMn()
};

// Matrices for Back Face

// Rotation Matrix (120 degrees)
Matrix4x4 RB = Matrix4x4(
	-0.5f,  -0.866f,  0.0f, -0.4873f,
	0.866f,  -0.5f,  0.0f, 0.8787f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
); // Row Major (GLSL is Column Major)

// Multiplication between Rotation and Translation Matrices
Matrix4x4 MultiplicationRBT = RB * T;
const Matrix RBT = {
	MultiplicationRBT.getUx(), MultiplicationRBT.getUy(), MultiplicationRBT.getUz(), MultiplicationRBT.getUn(),
	MultiplicationRBT.getVx(), MultiplicationRBT.getVy(), MultiplicationRBT.getVz(), MultiplicationRBT.getVn(),
	MultiplicationRBT.getWx(), MultiplicationRBT.getWy(), MultiplicationRBT.getWz(), MultiplicationRBT.getWn(),
	MultiplicationRBT.getMx(), MultiplicationRBT.getMy(), MultiplicationRBT.getMz(), MultiplicationRBT.getMn()
};

// Second multiplication between Rotation and Translation Matrices
Matrix4x4 MultiplicationRBT2 = MultiplicationRBT * MultiplicationRBT;
const Matrix RBT2 = {
	MultiplicationRBT2.getUx(), MultiplicationRBT2.getUy(), MultiplicationRBT2.getUz(), MultiplicationRBT2.getUn(),
	MultiplicationRBT2.getVx(), MultiplicationRBT2.getVy(), MultiplicationRBT2.getVz(), MultiplicationRBT2.getVn(),
	MultiplicationRBT2.getWx(), MultiplicationRBT2.getWy(), MultiplicationRBT2.getWz(), MultiplicationRBT2.getWn(),
	MultiplicationRBT2.getMx(), MultiplicationRBT2.getMy(), MultiplicationRBT2.getMz(), MultiplicationRBT2.getMn()
};


//Elements for the View Matrices

Vector3d eye = Vector3d(5.0f, 5.0f, 5.0f);
Vector3d center = Vector3d(0.0f, 0.0f, 0.0f);
Vector3d up = Vector3d(0.0f, 1.0f, 0.0f);

Vector3d view = center - eye;
Vector3d v = view.normalize();
Vector3d side = v * up;
Vector3d s = side.normalize();
Vector3d u = s * v;


//Elements for the Projection Matrices

GLfloat leftProj = -2.0f;
GLfloat rightProj = 2.0f;
GLfloat bottom = -2.0f;
GLfloat top = 2.0f;
GLfloat near = 1.0f;
GLfloat far = 10.0f;
GLfloat angle = 30;
GLfloat aspect = (GLfloat) width / height;

GLfloat tlr = (GLfloat) -(leftProj + rightProj) / 2.0f;
GLfloat tbt = (GLfloat) -(bottom + top) / 2.0f;
GLfloat tnf = (GLfloat) -(near + far) / 2.0f;

GLfloat slr = 2.0f / (rightProj - leftProj);
GLfloat sbt = 2.0f / (top - bottom);
GLfloat snf = 2.0f / (far - near);

//Translation Matrix
Matrix4x4 OrthographicTranslation = Matrix4x4::translation(tlr,tbt,tnf);

//Scalar Matrix 
Matrix4x4 OrthographicScaling = Matrix4x4::scale(slr, sbt, snf);

//Identity Matrix for the Orthographic Projection Matrix
Matrix4x4 Identity = Matrix4x4(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

// Orthographic Projection Matrix
Matrix4x4 OrthographicMultiplication = OrthographicScaling * OrthographicTranslation * Identity;
const Matrix Orthographic = {
	OrthographicMultiplication.getUx(), OrthographicMultiplication.getVx(), OrthographicMultiplication.getWx(), OrthographicMultiplication.getMx(),
	OrthographicMultiplication.getUy(), OrthographicMultiplication.getVy(), OrthographicMultiplication.getWy(), OrthographicMultiplication.getMy(),
	OrthographicMultiplication.getUz(), OrthographicMultiplication.getVz(), OrthographicMultiplication.getWz(), OrthographicMultiplication.getMz(),
	OrthographicMultiplication.getUn(), OrthographicMultiplication.getVn(), OrthographicMultiplication.getWn(), OrthographicMultiplication.getMn()
};

// Perspective Projection Matrix
Matrix4x4 PerspectiveMat = Matrix4x4::perspective(angle, aspect, near, far);
const Matrix Perspective = {
	PerspectiveMat.getUx(), PerspectiveMat.getVx(), PerspectiveMat.getWx(), PerspectiveMat.getMx(),
	PerspectiveMat.getUy(), PerspectiveMat.getVy(), PerspectiveMat.getWy(), PerspectiveMat.getMy(),
	PerspectiveMat.getUz(), PerspectiveMat.getVz(), PerspectiveMat.getWz(), PerspectiveMat.getMz(),
	PerspectiveMat.getUn(), PerspectiveMat.getVn(), PerspectiveMat.getWn(), PerspectiveMat.getMn()
};

void drawScene()
{
	// Rotation Matrix that aligns v with the -z axis
	Matrix4x4 ViewRotation = Matrix4x4(
		s.getX(), s.getY(), s.getZ(), 0.0f,
		u.getX(), u.getY(), u.getZ(), 0.0f,
		-v.getX(), -v.getY(), -v.getZ(), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	); // Column Major 

	// Translation Matrix
	Matrix4x4 EyeTranslation = Matrix4x4::translation(-eye.getX(), -eye.getY(), -eye.getZ());

	// Eye View Matrix
	Matrix4x4 EyeMultiplication = ViewRotation * EyeTranslation;

	Matrix Eye = {
		EyeMultiplication.getUx(), EyeMultiplication.getVx(), EyeMultiplication.getWx(), EyeMultiplication.getMx(),
		EyeMultiplication.getUy(), EyeMultiplication.getVy(), EyeMultiplication.getWy(), EyeMultiplication.getMy(),
		EyeMultiplication.getUz(), EyeMultiplication.getVz(), EyeMultiplication.getWz(), EyeMultiplication.getMz(),
		EyeMultiplication.getUn(), EyeMultiplication.getVn(), EyeMultiplication.getWn(), EyeMultiplication.getMn()
	};

	if (key_pressed == true) {
		glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);

		{
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix), Eye);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix), sizeof(Matrix), Orthographic);
		}
	}
	else {
		glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);

		{

			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix), Eye);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix), sizeof(Matrix), Perspective);
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	// Front face
	glUniformMatrix4fv(UniformId, 1, GL_TRUE, I);	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

	glUniformMatrix4fv(UniformId, 1, GL_TRUE, RT);	
	glDrawArrays(GL_TRIANGLE_STRIP, 12, 12);

	glUniformMatrix4fv(UniformId, 1, GL_TRUE, RT2);
	glDrawArrays(GL_TRIANGLE_STRIP, 24, 12);

	// Back face
	glUniformMatrix4fv(UniformId, 1, GL_TRUE, I);	
	glDrawArrays(GL_TRIANGLE_STRIP, 36, 12);

	glUniformMatrix4fv(UniformId, 1, GL_TRUE, RT);	
	glDrawArrays(GL_TRIANGLE_STRIP, 48, 12);

	glUniformMatrix4fv(UniformId, 1, GL_TRUE, RT2);
	glDrawArrays(GL_TRIANGLE_STRIP, 60, 12);

	glUseProgram(0);
	glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////// Mouse Control

Vector2d oldMousePosition = Vector2d(0.0f, 0.0f);

void mouseUpdate(Vector2d newMousePosition) {
	Vector2d mouseDelta = newMousePosition - oldMousePosition;
	if (oldMousePosition.getX() != 0 && oldMousePosition.getY() != 0){
		eye.setX(eye.getX() + mouseDelta.getX()*0.01f);
		eye.setY(eye.getY() + mouseDelta.getY()*0.01f);
	}
	oldMousePosition = newMousePosition;
	drawScene();
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

///////////////////////////////////////////////////////////////////////// SETUP

GLFWwindow* setupWindow(int winx, int winy, const char* title,
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);
	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
}

GLFWwindow* setup(int major, int minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win =
		setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);
	setupErrorCallback();
	createShaderProgram();
	createBufferObjects();
	return win;
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec)
{
	drawScene();
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// AUXILIARY FUNCTIONS


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		key_pressed = !key_pressed;
	}
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		eye.setZ(eye.getZ() - (GLfloat) 0.1f);
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		eye.setX(eye.getX() - (GLfloat) 0.1f);
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		eye.setZ(eye.getZ() + (GLfloat) 0.1f);
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		eye.setX(eye.getX() + (GLfloat) 0.1f);
	}

	drawScene();
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	Vector2d newMousePosition = Vector2d((GLfloat)xpos, (GLfloat)ypos);
	mouseUpdate(newMousePosition);
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;
	
	GLFWwindow* win = setup(gl_major, gl_minor,
		width, height, "Hello Modern 3D World", is_fullscreen, is_vsync);
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	run(win);
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////
