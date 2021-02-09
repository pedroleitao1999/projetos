///////////////////////////////////////////////////////////////////////////////
//
// ASSIGNMENT 6
//
// Group 12
//
// 84741 - Marc Alexandre Jelkic
// 90764 - Pedro de Oliveira Rosa Alves Leitao
//
// Debug in x86
//
///////////////////////////////////////////////////////////////////////////////

#include "main.h"

GLuint VaoId, VboId[2];
GLuint VertexShaderId, FragmentShaderId;
GLint UboId, UniformId;
bool key_pressed_P = false;
bool key_pressed_G = false; // True for Quaternions, false for Euler
bool key_pressed_N = false; // True for moving, false for stop
int width = 640;
int height = 480;
const float vThreshold = (float)1.0e-5;
const float mThreshold = (float)1.0e-5;
const float qThreshold = (float)1.0e-5;
using namespace std;

Loader loaderGlobal = Loader();
Mesh *meshGlobal;
Scene sceneGlobal;
Shader shaderGlobal;

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

/////////////////////////////////////////////////////////////////////// MOUSE CONTROLS

Vector2d oldMousePosition = Vector2d(0.0f, 0.0f);

void mouseUpdate(Vector2d newMousePosition) {
	Vector2d mouseDelta = newMousePosition - oldMousePosition;
	if (oldMousePosition.getX() != 0 && oldMousePosition.getY() != 0) {
		sceneGlobal.getCamera().orientateCamera(mouseDelta);
	}
	oldMousePosition = newMousePosition;
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	shaderGlobal.destroyShaderProgram();
	sceneGlobal.destroyBufferObjects(VaoId);
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
	GLFWwindow* win = setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);
	setupErrorCallback();

	// Loading obj files and creating mesh
	std::string mesh_dir = "assets/models/";
	std::string mesh_file = "cube.obj";
	std::string mesh_fullname = mesh_dir + mesh_file;
	meshGlobal = loaderGlobal.createMesh(mesh_fullname);

	// Loading shader files and creating shader program
	shaderGlobal = Shader(meshGlobal);
	std::string shader_dir = "assets/shader/";
	std::string vs = shader_dir + "cube_vs.glsl";
	std::string fs = shader_dir + "cube_fs.glsl";
	shaderGlobal.createShaderProgram(vs, fs);

	// Scene creation with scenegraph
	sceneGlobal = Scene(meshGlobal, shaderGlobal, VaoId);
	sceneGlobal.createCubes();
	sceneGlobal.getSceneGraph().createBufferObjectsSceneGraph(VaoId);

	return win;
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec)
{
	sceneGlobal.getSceneGraph().drawSceneGraph(VaoId);
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
	Camera cam = sceneGlobal.getCamera();
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		key_pressed_P = !key_pressed_P;
	} 
	else if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		// Switch de Euler para Quaternions
		key_pressed_G = !key_pressed_G;
	}	
	else if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		// Trigger as animacoes
		key_pressed_N = !key_pressed_N;
	}

	/// CAMERA CONTROLS
	else if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		GLfloat newZ = cam.getEye().getZ() - (GLfloat)0.1f;
		Vector3d eye = cam.getEye();
		Vector3d newEye = Vector3d(cam.getEye().getX(), cam.getEye().getY(), newZ);
		cam.moveCamera(newEye);
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		GLfloat newX = cam.getEye().getX() - (GLfloat)0.1f;
		Vector3d eye = cam.getEye();
		Vector3d newEye = Vector3d(newX, cam.getEye().getY(), cam.getEye().getZ());
		cam.moveCamera(newEye);
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		GLfloat newZ = cam.getEye().getZ() + (GLfloat)0.1f;
		Vector3d eye = cam.getEye();
		Vector3d newEye = Vector3d(cam.getEye().getX(), cam.getEye().getY(), newZ);
		cam.moveCamera(newEye);
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		GLfloat newX = cam.getEye().getX() + (GLfloat)0.1f;
		Vector3d eye = cam.getEye();
		Vector3d newEye = Vector3d(newX, cam.getEye().getY(), cam.getEye().getZ());
		cam.moveCamera(newEye);
	}
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
		width, height, "Sixth Assignment", is_fullscreen, is_vsync);
	// KEY CONTROLS
	glfwSetKeyCallback(win, key_callback);
	// MOUSE CONTROLS
	glfwSetCursorPosCallback(win, mouse_callback);
	run(win);
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////