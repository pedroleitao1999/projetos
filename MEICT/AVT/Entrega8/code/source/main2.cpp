///////////////////////////////////////////////////////////////////////////////
//
// Adding a camera and moving to the 3rd dimension.
// A "Hello 3D World" of Modern OpenGL.
//
// (c) 2013-20 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <stack>
#include "HeaderFiles/Matrix4D.h"
#include "HeaderFiles/Matrix3D.h"
#include "HeaderFiles/Vec3D.h"
#include "HeaderFiles/Camera.h"
#include "HeaderFiles/Renderer.h"
#include "HeaderFiles/Color.h"
#include "HeaderFiles/Quat.h"
#include "HeaderFiles/Scene.h"
#include "HeaderFiles/Mesh.h"
#include "HeaderFiles/Shader.h"
#include "HeaderFiles/BlinnPhongTerm.h"

#define VERTICES 0
#define COLORS 1

GLuint VaoId, VboId[3];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UboId, UniformId;
const GLuint UBO_BP = 0;
bool rotationOn = false;
bool animationOn = false;
Renderer *renderer = nullptr;

GLfloat animationStart = 0;
const GLfloat animationDuration = 3;

using namespace std;

#ifndef __APPLE__
#define ERROR_CALLBACK // MacOS uses OpenGL 4.1
#endif

////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

#ifdef ERROR_CALLBACK // OpenGL 4.3 Error Handling

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
    glDebugMessageCallback(error, 0);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
    // params: source, type, severity, count, ids, enabled
}

#endif

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

void createBufferObjects()
{
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    {
        glGenBuffers(3, VboId);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
        {
            glEnableVertexAttribArray(VERTICES);
            glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
            glEnableVertexAttribArray(COLORS);
            glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(GLfloat) * 4));
        }
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
    {
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix) * 2, nullptr, GL_STREAM_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, UBO_BP, VboId[1]);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void destroyBufferObjects()
{
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(VERTICES);
    glDisableVertexAttribArray(COLORS);
    glDeleteBuffers(3, VboId);
    glDeleteVertexArrays(1, &VaoId);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////// SCENE

Matrix4D MM({
        1.0f,  0.0f,  0.0f,  0.0f,
        0.0f,  1.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  1.0f,  0.0f,
        -0.0f, -0.0f, -0.0f,  1.0f
}); // Column Major

std::map<char, Matrix4D> MR {
    { 'x', Matrix4D::identity() },
    { 'y', Matrix4D::identity() },
    { 'z', Matrix4D::identity() }
};

Matrix4D RM = MM;

Matrix4D IM = MM;


Vec3D eye(-15, 15, 15);
Vec3D center(0, 0, 0);
Vec3D up(0, 1, 0);

////// Blinn–Phong components //////
GLfloat ambientMaterial = 0.7f;
GLfloat diffuseMaterial = 0.4f;
GLfloat specularMaterial = 0.1f;

Vec3D ambientLight(1.0f, 1.0f, 1.0f);
Vec3D diffuseLight(1.0f, 1.0f, 1.0f);
Vec3D specularLight(1.0f, 1.0f, 1.0f);

GLfloat ambientFactor = 1.0f;

Vec3D lightPosition(30, 20, 25);
double shininess = 16.0;

BlinnPhongTerm ambient = BlinnPhongTerm(ambientMaterial, ambientLight, ambientFactor);
BlinnPhongTerm diffuse = BlinnPhongTerm(diffuseMaterial, diffuseLight, 0.0f);
BlinnPhongTerm specular = BlinnPhongTerm(specularMaterial, specularLight, 0.0f);

GLfloat positionalLight[] = { lightPosition.getX(), lightPosition.getY(), lightPosition.getZ(), 1.0f };
GLfloat diffuseRGBA[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specularRGBA[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Camera
Camera camera({ eye,center,up });

Vec4D axisX({ 1.0f, 0.0f, 0.0f, 1.0f });
Vec4D axisY({ 0.0f, 1.0f, 0.0f, 1.0f });
Vec4D axisZ({ 0.0f, 0.0f, 1.0f, 1.0f });

Quat qx = Quat::qFromAngleAxis(0, axisX);
Quat qy = Quat::qFromAngleAxis(0, axisY);
Quat qz = Quat::qFromAngleAxis(0, axisZ);

GLfloat qRotation = 0.0f;


// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
Matrix4D Ortographic = Camera::ortographicP(-8.0f, 8.0f, -8.0f, 8.0f, 1.0f, 50.0f);

GLfloat* OrtographicGL = Ortographic.toGLfloats();

// Perspective Fovy(30) Aspect(640/480)= 1.33 NearZ(1) FarZ(10)
Matrix4D Perspective = Camera::perspectiveP(40.0f, 1.33f, 1.0f, 50.0f);

GLfloat* PerspectiveGL = Perspective.toGLfloats();

GLfloat* P = OrtographicGL;
Vec3D negativeV = camera.getNegativeV();
Matrix3D View3D({ camera.getS(), camera.getU(), negativeV });
Matrix4D RotationV = View3D.convertTo4D();
Matrix4D ViewJ = camera.convertToView(View3D,Vec3D(-camera.getS().dot(camera.getEye()), -camera.getU().dot(camera.getEye()), camera.getV().dot(camera.getEye())));

Matrix4D ViewM = ViewJ.translate(-1.0f, 0.0f, 0.0f).mul(ViewJ);

Matrix4D MB = RM;

// Player1 
Matrix4D M0 = IM * RM * Matrix4D::translate(-5.25f, 0.0f, -5.25f);
Matrix4D M1 = IM * M0 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D M2 = IM * M1 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D M3 = IM * M2 * Matrix4D::translate(3.0f, 0.0f, 0.0f);

Matrix4D M4 = IM * M0 * Matrix4D::translate(1.5f, 0.0f, 1.5f);
Matrix4D M5 = IM * M4 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D M6 = IM * M5 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D M7 = IM * M6 * Matrix4D::translate(3.0f, 0.0f, 0.0f);

Matrix4D M8 = IM * M4 * Matrix4D::translate(-1.5f, 0.0f, 1.5f);
Matrix4D M9 = IM * M8 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D M10 = IM * M9 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D M11 = IM * M10 * Matrix4D::translate(3.0f, 0.0f, 0.0f);

// Player2
Matrix4D MN0 = IM * M8 * Matrix4D::translate(1.5f, 0.0f, 4.5f);
Matrix4D MN1 = IM * MN0 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D MN2 = IM * MN1 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D MN3 = IM * MN2 * Matrix4D::translate(3.0f, 0.0f, 0.0f);

Matrix4D MN4 = IM * MN0 * Matrix4D::translate(-1.5f, 0.0f, 1.5f);
Matrix4D MN5 = IM * MN4 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D MN6 = IM * MN5 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D MN7 = IM * MN6 * Matrix4D::translate(3.0f, 0.0f, 0.0f);

Matrix4D MN8 = IM * MN4 * Matrix4D::translate(1.5f, 0.0f, 1.5f);
Matrix4D MN9 = IM * MN8 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D MN10 = IM * MN9 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
Matrix4D MN11 = IM * MN10 * Matrix4D::translate(3.0f, 0.0f, 0.0f);

////// CREATE SCENE GRAPH //////
SceneGraph createSceneGraph() {
    SceneGraph graph = SceneGraph(camera);

    // Loading obj files and creating mesh
    std::string piece_mesh_fullname = "piece.obj";
    Mesh piece_mesh = Mesh(piece_mesh_fullname);

    std::string board_mesh_fullname = "board.obj";
    Mesh board_mesh = Mesh(board_mesh_fullname);
    
    // Loading shader files and creating shader program
    Shader shader = Shader(&piece_mesh);

    //Board Node
    SceneNode board = SceneNode(board_mesh, MB, shader);
    graph.createNode(board);

    //Pieces Node
    SceneNode pieces = SceneNode(piece_mesh, M0, shader);
    
    SceneNode player1pieces = SceneNode(piece_mesh, M0, shader);
    SceneNode player2pieces = SceneNode(piece_mesh, MN0, shader);

    //Player1
    SceneNode piece0 = SceneNode(piece_mesh, M0, shader);
    player1pieces.addChild(piece0);
    SceneNode piece1 = SceneNode(piece_mesh, M1, shader);
    player1pieces.addChild(piece1);
    SceneNode piece2 = SceneNode(piece_mesh, M2, shader);
    player1pieces.addChild(piece2);
    SceneNode piece3 = SceneNode(piece_mesh, M3, shader);
    player1pieces.addChild(piece3);

    SceneNode piece4 = SceneNode(piece_mesh, M4, shader);
    player1pieces.addChild(piece4);
    SceneNode piece5 = SceneNode(piece_mesh, M5, shader);
    player1pieces.addChild(piece5);
    SceneNode piece6 = SceneNode(piece_mesh, M6, shader);
    player1pieces.addChild(piece6);
    SceneNode piece7 = SceneNode(piece_mesh, M7, shader);
    player1pieces.addChild(piece7);

    SceneNode piece8 = SceneNode(piece_mesh, M8, shader);
    player1pieces.addChild(piece8);
    SceneNode piece9 = SceneNode(piece_mesh, M9, shader);
    player1pieces.addChild(piece9);
    SceneNode piece10 = SceneNode(piece_mesh, M10, shader);
    player1pieces.addChild(piece10);
    SceneNode piece11 = SceneNode(piece_mesh, M11, shader);
    player1pieces.addChild(piece11);

    //Player2
    SceneNode pieceN0 = SceneNode(piece_mesh, MN0, shader);
    player2pieces.addChild(pieceN0);
    SceneNode pieceN1 = SceneNode(piece_mesh, MN1, shader);
    player2pieces.addChild(pieceN1);
    SceneNode pieceN2 = SceneNode(piece_mesh, MN2, shader);
    player2pieces.addChild(pieceN2);
    SceneNode pieceN3 = SceneNode(piece_mesh, MN3, shader);
    player2pieces.addChild(pieceN3);

    SceneNode pieceN4 = SceneNode(piece_mesh, MN4, shader);
    player2pieces.addChild(pieceN4);
    SceneNode pieceN5 = SceneNode(piece_mesh, MN5, shader);
    player2pieces.addChild(pieceN5);
    SceneNode pieceN6 = SceneNode(piece_mesh, MN6, shader);
    player2pieces.addChild(pieceN6);
    SceneNode pieceN7 = SceneNode(piece_mesh, MN7, shader);
    player2pieces.addChild(pieceN7);

    SceneNode pieceN8 = SceneNode(piece_mesh, MN8, shader);
    player2pieces.addChild(pieceN8);
    SceneNode pieceN9 = SceneNode(piece_mesh, MN9, shader);
    player2pieces.addChild(pieceN9);
    SceneNode pieceN10 = SceneNode(piece_mesh, MN10, shader);
    player2pieces.addChild(pieceN10);
    SceneNode pieceN11 = SceneNode(piece_mesh, MN11, shader);
    player2pieces.addChild(pieceN11);

    pieces.addChild(player1pieces);
    pieces.addChild(player2pieces);

    graph.createNode(pieces);

    return graph;
}

GLfloat lerp(GLfloat a, GLfloat b, GLfloat t) {
    if(a<=0 && b>=0 || a>=0 && b<=0) return t*b + (1-t)*a;
    if(t==1) return b;
    const GLfloat x = a + t*(b-a);
    return t>1 == b>a ? max(b,x) : min(b,x);
}

typedef struct {
    char axis;
    GLfloat time;
} Keyframe;

int keyframesAmount = 3;
Keyframe keyframes[] = {
    {'y', 0},
    {'x', 3},
    {'z', 6},
    {'0', 9}
};

std::map<char, GLfloat> rotations {
        {'x', 0},
        {'y', 0},
        {'z', 0},
};

const char AXES[3] = {'x', 'y', 'z'};

int activeKeyframe = 0;

void animation() {
    //Euler animation rotation

    GLfloat currentTime = (GLfloat)glfwGetTime();
    GLfloat animationProgress = currentTime - animationStart;
    if (animationOn) {
        

            if (animationProgress > keyframes[activeKeyframe + 1].time) {
                activeKeyframe = activeKeyframe + 1;
                if (activeKeyframe >= keyframesAmount) {
                    activeKeyframe = 0;
                    animationOn = false;
                    return;
                }
            }

            GLfloat animationPercentage = (animationProgress - keyframes[activeKeyframe].time) / animationDuration;

            rotations[keyframes[activeKeyframe].axis] = lerp(
                0.0f,
                90.0f,
                animationPercentage);
            for (char axis : AXES) {
                MR[axis] = Matrix4D::rotate(rotations[axis], axis);
            }
            IM = MM * MR['z'] * MR['y'] * MR['x'];
        
        
        
    }
    
    if (rotationOn) {
        RM = RM * Matrix4D::rotate((GLfloat)0.2, 'y');
    }

    
}

void drawScene()
{

    SceneGraph sceneGraph = createSceneGraph();
    
    GLfloat* ViewMGL = ViewM.toGLfloats();
    glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix), ViewMGL);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix), sizeof(Matrix), P);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Drawing directly in clip space

    animation();

    //glBindVertexArray(VaoId);
    glUseProgram(ProgramId);

    if (!renderer) {
        renderer = new Renderer(VaoId, VboId, UniformId);
    }

    for (int i = 0; i < sceneGraph.getNodeNumber(); i++) {
        vector<SceneNode> all_nodes = sceneGraph.getNodes();
        SceneNode node = all_nodes[i];
        Mesh mesh = node.getMesh();
        if (node.getChildrenNumber() > 0) {
            //Pieces
            for (int j = 0; j < node.getChildrenNumber(); j++) {
                vector<SceneNode> all_children = node.getChildren();
                SceneNode child = all_children[j];
                if (child.getChildrenNumber() > 0) {
                    for (int k = 0; k < child.getChildrenNumber(); k++) {
                        vector<SceneNode> all_children2 = child.getChildren();
                        SceneNode child2 = all_children2[k];
                        if (j == 0)
                            renderer->drawTriangles(mesh.getVertices(), child2.generateColors(1.0f, 0.0f, 0.0f), mesh.getVertexIdx(), mesh.getNormals(), child2.getMatrix(),
                                ambient, diffuse, specular, lightPosition, eye, shininess);
                        else
                            renderer->drawTriangles(mesh.getVertices(), child2.generateColors(0.0f, 0.0f, 1.0f), mesh.getVertexIdx(), mesh.getNormals(), child2.getMatrix(),
                                ambient, diffuse, specular, lightPosition, eye, shininess);
                    }
                }
            }
        }
        else {
            //Board
            renderer->drawTriangles(mesh.getVertices(), node.generateColors(0.59f, 0.29f, 0.0f), mesh.getVertexIdx(), mesh.getNormals(), node.getMatrix(),
                ambient, diffuse, specular, lightPosition, eye, shininess);
        }
    }
       
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

#ifdef __APPLE__ // Necessary Window Hints for MacOS
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

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
    glfwGetFramebufferSize(win, &winx, &winy);
    setupGLEW();
    setupOpenGL(winx, winy);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, positionalLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseRGBA);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 30.0f);
#ifdef ERROR_CALLBACK
    setupErrorCallback();
#endif
    //SceneGraph sceneGraph = createSceneGraph();
    createShaderProgram();
    createBufferObjects();
    return win;
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec)
{
    drawScene();
    
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GLfloat axisX = 0;
    GLfloat axisY = 0;
    GLfloat axisZ = 0;
    Vec3D movement(0, 0, 0);
    if (key == GLFW_KEY_A)ViewM = ViewM.translate(0.5f, 0.0f, 0.0f).mul(ViewM);
    if (key == GLFW_KEY_D) ViewM = ViewM.translate(-0.5f, 0.0f, 0.0f).mul(ViewM);

    if (key == GLFW_KEY_S) ViewM = ViewM.translate(0.0f, 0.0f, -0.5f).mul(ViewM);
    if (key == GLFW_KEY_W) ViewM = ViewM.translate(0.0f, 0.0f, 0.5f).mul(ViewM);

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_P) if (P == OrtographicGL) { P = PerspectiveGL; }
        else { P = OrtographicGL; };

        if (key == GLFW_KEY_F) { rotationOn = !rotationOn; }
        if (key == GLFW_KEY_C) {
            
            if (!animationOn) {
                animationOn = true;
                animationStart = (GLfloat)glfwGetTime();
                for (char axis : AXES) {
                    rotations[axis] = 0.0f;
                }
            }
            
        }
    }
}


GLfloat lastX;
GLfloat lastY;
bool firstMouse = true;

GLfloat rads(GLfloat degrees) {
    return (GLfloat)(degrees * M_PI / 180);
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
    
    if (firstMouse)
    {
        lastX = (GLfloat)xpos;
        lastY = (GLfloat)ypos;
        firstMouse = false;
    }

    GLfloat xoffset = (GLfloat)xpos - lastX;
    GLfloat yoffset = (GLfloat)ypos- lastY;
    lastX = (GLfloat)xpos;
    lastY = (GLfloat)ypos;

    GLfloat sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    ViewM = ViewM.rotate(xoffset, 'y').mul(ViewM);
    ViewM = ViewM.rotate(yoffset, 'x').mul(ViewM);

}



void run(GLFWwindow* win)
{
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(win))
    {
        double time = glfwGetTime();
        double elapsed_time = time - last_time;
        last_time = time;
        glfwSetCursorPosCallback(win, mouse_callback);
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetKeyCallback(win, key_callback);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        display(win, elapsed_time);
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwDestroyWindow(win);
    glfwTerminate();
}

////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
#ifdef __APPLE__
    int gl_major = 4, gl_minor = 1; // Using OpenGL 4.1 on MacOS
#else
    int gl_major = 4, gl_minor = 3;
#endif
    int is_fullscreen = 0;
    int is_vsync = 1;
    GLFWwindow* win = setup(gl_major, gl_minor,
        640, 480, "Hello Modern 3D World", is_fullscreen, is_vsync);
    run(win);
    exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////