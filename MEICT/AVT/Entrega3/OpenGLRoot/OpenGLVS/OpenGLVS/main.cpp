///////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// (c)2013-20 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vector.h"
#include "matrix.h"

#define VERTICES 0
#define COLORS 1

GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK

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
	std::cerr << "  debug call: " << std::endl << message << std::endl;
	std::cerr << "Press <return>.";
	std::cin.ignore();
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	// glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
	// params: source, type, severity, count, ids, enabled
}

#else // ERROR_CALLBACK

///////////////////////////////////////////////// ERROR HANDLING (All versions)

static const std::string errorString(GLenum error)
{
	switch(error) {
	case GL_NO_ERROR:
		return "No error has been recorded.";
	case GL_INVALID_ENUM:
		return "An unacceptable value is specified for an enumerated argument.";
	case GL_INVALID_VALUE:
		return "A numeric argument is out of range.";
	case GL_INVALID_OPERATION:
		return "The specified operation is not allowed in the current state.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "The framebuffer object is not complete.";
	case GL_OUT_OF_MEMORY:
		return "There is not enough memory left to execute the command.";
	case GL_STACK_UNDERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
	case GL_STACK_OVERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
	default: exit(EXIT_FAILURE);
	}
}

static bool isOpenGLError() 
{
	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		std::cerr << "OpenGL ERROR [" << errorString(errCode) << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

#endif // ERROR_CALLBACK

/////////////////////////////////////////////////////////////////////// SHADERs

const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec4 in_Position;\n"
	"in vec4 in_Color;\n"
	"out vec4 ex_Color;\n"

	"uniform mat4 Matrix;\n"

	"void main(void)\n"
	"{\n"
	"	gl_Position = Matrix * in_Position;\n"
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
	// Shader dos vértices
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	// Shader das cores
	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	// Criacao do programa
	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	// Binding dos shaders no programa
	glBindAttribLocation(ProgramId, VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, COLORS, "in_Color");

	// Linking do progama e criacao do Uniform
	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	// Detach e Delete dos shaders
	glDetachShader(ProgramId, VertexShaderId);
	glDeleteShader(VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);
	glDeleteShader(FragmentShaderId);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create shaders.");
#endif
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(ProgramId);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy shaders.");
#endif
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct
{
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] =
{

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
	{{ 0.42f, -0.37f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.3f, 1.0f }}

};

const GLubyte Indices[] =
{
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
	33,34,35
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
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
}

void destroyBufferObjects()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
#endif
}

/////////////////////////////////////////////////////////////////////// SCENE

typedef GLfloat Matrix[16];

// Identity Matrix
const Matrix I = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

const Matrix M = {
	1.0f,  0.0f,  0.0f, -1.0f,
	0.0f,  1.0f,  0.0f, -1.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

// Rotation Matrix (120 degrees / )
const Matrix R = {
	-0.5f,  0.866f,  0.0f, -0.4873f,
	-0.866f,  -0.5f,  0.0f, 0.8787f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

// Translation Matrix
const Matrix T = {
	1.0f,  0.0f,  0.0f, 0.55f,
	0.0f,  1.0f,  0.0f, 1.185f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

const Matrix RT = {
	-0.5f,  0.866f,  0.0f, 0.2639f,
	-0.866f, -0.5f,  0.0f, -0.1901f,
	0.0f,  0.0f,  1.0f, 0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

const Matrix RT2 = {
	-0.5f,  -0.866f,  0.0f, -0.03268f,
	0.866f, -0.5f,  0.0f, -0.3236f,
	0.0f,  0.0f,  1.0f, 0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

void drawScene()
{
	// Drawing directly in clip space

	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	// Normal
	glUniformMatrix4fv(UniformId, 1, GL_TRUE, I);
	glDrawElements(GL_TRIANGLE_STRIP, 12, GL_UNSIGNED_BYTE, (GLvoid*)0);

	// Translacionado
	glUniformMatrix4fv(UniformId, 1, GL_TRUE, RT);
	glDrawElements(GL_TRIANGLE_STRIP, 24, GL_UNSIGNED_BYTE, (GLvoid*)0);

	// Translacionado
	glUniformMatrix4fv(UniformId, 1, GL_TRUE, RT2);
	glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not draw scene.");
#endif
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
#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
	// Programa em si é chamado aqui
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

		// Double Buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		display(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		checkOpenGLError("ERROR: MAIN/RUN");
#endif
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;
	// Esta funcao "setup" é o ponto de partida
	GLFWwindow* win = setup(gl_major, gl_minor, 
		640, 480, "Penrose Triangle", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END