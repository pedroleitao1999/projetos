///////////////////////////////////////////////////////////////////////////////
//
// Adding a camera and moving to the 3rd dimension.
// A "Hello 3D World" of Modern OpenGL.
//
// (c) 2013-20 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <stack>
#include <ctime>
#include <chrono>
#include <string> 
#include <iomanip>
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
#include "HeaderFiles/Light.h"

#define STB_IMAGE_IMPLEMENTATION
#include "HeaderFiles/stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "HeaderFiles/stb_image_write.h"

#define POSITIONS 0
#define NORMALS 1

#define LIGHT_POSITION 2
#define LIGHT_AMBIENT_COLOR 3
#define LIGHT_DIFFUSE_COLOR 4
#define LIGHT_SPECULAR_COLOR 5

#define MATERIAL_EMISSIVE_COLOR 6
#define MATERIAL_AMBIENT_COLOR 7
#define MATERIAL_DIFFUSE_COLOR 8
#define MATERIAL_SPECULAR_COLOR 9
#define MATERIAL_SHININESS 10

#define PAINTED_COLOR 11
#define TEXCOORD 12
#define LIGHT_POWER 13

GLuint VaoId, VboId[3];
GLuint ProgramId, VertexShaderId, FragmentShaderId;
GLint UboId, UniformId;
unsigned int TextureId;
unsigned int NormalId;
GLint posAttrib;
const GLuint UBO_BP = 0;
bool rotationOn = false;
bool animationOn = false;
Renderer *renderer = nullptr;
GLFWwindow* window;

Vec3D WoodMaterial(0.79f, 0.64f, 0.45f);
Vec3D black(0.0f, 0.0f, 0.0f);
Vec3D white(1.0f, 1.0f, 1.0f);

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

typedef struct {
    GLfloat position[4];
    GLfloat normal[4];
    GLfloat light_position[4];
    GLfloat light_ambient[4];
    GLfloat light_diffuse[4];
    GLfloat light_specular[4];
    GLfloat material_emissive[4];
    GLfloat material_ambient[4];
    GLfloat material_diffuse[4];
    GLfloat material_specular[4];
    GLfloat material_shininess;
    GLfloat painted_color[4];
    GLfloat texcoord[2];
    GLfloat light_power;
} Info;

typedef GLfloat Matrix[16];

const GLuint checkCompilation(const GLuint shader_id, const std::string& filename)
{
    GLint compiled;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        GLchar* const log = new char[length];
        glGetShaderInfoLog(shader_id, length, &length, log);
        std::cerr << "[" << filename << "] " << std::endl << log;
        delete[] log;
        exit(EXIT_FAILURE);
    }
    return compiled;
}

void checkLinkage(const GLuint program_id) {
    GLint linked;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
        GLchar* const log = new char[length];
        glGetProgramInfoLog(program_id, length, &length, log);
        std::cerr << "[LINK] " << std::endl << log << std::endl;
        delete[] log;
        exit(EXIT_FAILURE);
    }
}

void useBuffer() {
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);
    {
        glGenBuffers(3, VboId);

        glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
        {
            glEnableVertexAttribArray(POSITIONS);
            glVertexAttribPointer(POSITIONS, 4, GL_FLOAT, GL_FALSE, sizeof(Info), nullptr);
            glEnableVertexAttribArray(NORMALS);
            glVertexAttribPointer(NORMALS, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 4));
            glEnableVertexAttribArray(LIGHT_POSITION);
            glVertexAttribPointer(LIGHT_POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 8));
            glEnableVertexAttribArray(LIGHT_AMBIENT_COLOR);
            glVertexAttribPointer(LIGHT_AMBIENT_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 12));
            glEnableVertexAttribArray(LIGHT_DIFFUSE_COLOR);
            glVertexAttribPointer(LIGHT_DIFFUSE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 16));
            glEnableVertexAttribArray(LIGHT_SPECULAR_COLOR);
            glVertexAttribPointer(LIGHT_SPECULAR_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 20));
            glEnableVertexAttribArray(MATERIAL_EMISSIVE_COLOR);
            glVertexAttribPointer(MATERIAL_EMISSIVE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 24));
            glEnableVertexAttribArray(MATERIAL_AMBIENT_COLOR);
            glVertexAttribPointer(MATERIAL_AMBIENT_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 28));
            glEnableVertexAttribArray(MATERIAL_DIFFUSE_COLOR);
            glVertexAttribPointer(MATERIAL_DIFFUSE_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 32));
            glEnableVertexAttribArray(MATERIAL_SPECULAR_COLOR);
            glVertexAttribPointer(MATERIAL_SPECULAR_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 36));
            glEnableVertexAttribArray(MATERIAL_SHININESS);
            glVertexAttribPointer(MATERIAL_SHININESS, 1, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 40));
            glEnableVertexAttribArray(PAINTED_COLOR);
            glVertexAttribPointer(PAINTED_COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 41));
            glEnableVertexAttribArray(TEXCOORD);
            glVertexAttribPointer(TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 45));
            glEnableVertexAttribArray(LIGHT_POWER);
            glVertexAttribPointer(LIGHT_POWER, 1, GL_FLOAT, GL_FALSE, sizeof(Info), (GLvoid*)(sizeof(GLfloat) * 47));
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


void createShaderProgram()
{
    // Loading fragment shader from file
    std::ifstream inf("base.frag");
    std::string fragShaderCode((std::istreambuf_iterator<char>(inf)), std::istreambuf_iterator<char>());
    auto* FragmentShader = fragShaderCode.c_str();

    // Loading vertex shader from file
    std::ifstream inv("base.vert");
    std::string vertShaderCode((std::istreambuf_iterator<char>(inv)), std::istreambuf_iterator<char>());
    auto* VertexShader = vertShaderCode.c_str();

    useBuffer();

    VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderId, 1, &VertexShader, nullptr);
    glCompileShader(VertexShaderId);
    checkCompilation(VertexShaderId, VertexShader);

    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderId, 1, &FragmentShader, nullptr);
    glCompileShader(FragmentShaderId);
    checkCompilation(FragmentShaderId, FragmentShader);

    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);

    glBindFragDataLocation(ProgramId, 0, "outColor");

    glLinkProgram(ProgramId);
    checkLinkage(ProgramId);

    glDetachShader(ProgramId, VertexShaderId);
    glDeleteShader(VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);
    glDeleteShader(FragmentShaderId);

    // Setting flags for textures
    glGenTextures(1, &TextureId);
    glGenTextures(1, &NormalId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glUseProgram(ProgramId);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureId);

    // Loading image
    stbi_set_flip_vertically_on_load(1);
    int x, y, p;
    unsigned char* TextureBuffer = stbi_load("diffuse.jpg", &x, &y, &p, 0);
    if (TextureBuffer)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Freeing image
    stbi_image_free(TextureBuffer);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, NormalId);

    // Loading image
    stbi_set_flip_vertically_on_load(1);
    TextureBuffer = stbi_load("normal.jpg", &x, &y, &p, 0);
    if (TextureBuffer)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Freeing image
    stbi_image_free(TextureBuffer);

    glUniform1i(glGetUniformLocation(ProgramId, "in_Texture"), 0);
    glUniform1i(glGetUniformLocation(ProgramId, "in_NormalMap"), 1);
}

void destroyShaderProgram()
{
    glUseProgram(0);
    glDeleteProgram(ProgramId);
}

/////////////////////////////////////////////////////////////////// VAOs & VBOs

void destroyBufferObjects()
{
    glBindVertexArray(VaoId);
    glDisableVertexAttribArray(POSITIONS);
    glDisableVertexAttribArray(NORMALS);
    glDisableVertexAttribArray(LIGHT_POSITION);
    glDisableVertexAttribArray(LIGHT_AMBIENT_COLOR);
    glDisableVertexAttribArray(LIGHT_DIFFUSE_COLOR);
    glDisableVertexAttribArray(LIGHT_SPECULAR_COLOR);
    glDisableVertexAttribArray(MATERIAL_EMISSIVE_COLOR);
    glDisableVertexAttribArray(MATERIAL_AMBIENT_COLOR);
    glDisableVertexAttribArray(MATERIAL_DIFFUSE_COLOR);
    glDisableVertexAttribArray(MATERIAL_SPECULAR_COLOR);
    glDisableVertexAttribArray(MATERIAL_SHININESS);
    glDisableVertexAttribArray(PAINTED_COLOR);
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

Vec3D eye(0, 5, -16);
Vec3D center(0, 0, 0);
Vec3D up(0, 1, 0);

Vec3D lightPosition(0.0f, 3.0f, 0.0f);
Vec3D ambientLight(1, 1, 1);
Vec3D diffuseLight(1, 1, 1);
Vec3D specularLight(1, 1, 1);
GLfloat light_power = 4.0f;
Light light = Light(lightPosition, ambientLight, diffuseLight, specularLight, light_power);

double shininess = 0.3;

// Camera
Camera camera({ eye,center,up });

Vec4D axisX({ 1.0f, 0.0f, 0.0f, 1.0f });
Vec4D axisY({ 0.0f, 1.0f, 0.0f, 1.0f });
Vec4D axisZ({ 0.0f, 0.0f, 1.0f, 1.0f });

Quat qx = Quat::qFromAngleAxis(0, axisX);
Quat qy = Quat::qFromAngleAxis(0, axisY);
Quat qz = Quat::qFromAngleAxis(0, axisZ);

GLfloat qRotation = 0.0f;

Matrix4D Ortographic = Camera::ortographicP(-8.0f, 8.0f, -8.0f, 8.0f, 1.0f, 50.0f);

GLfloat* OrtographicGL = Ortographic.toGLfloats();

Matrix4D Perspective = Camera::perspectiveP(40.0f, 1.33f, 1.0f, 50.0f);

GLfloat* PerspectiveGL = Perspective.toGLfloats();

GLfloat* P = OrtographicGL;
Vec3D negativeV = camera.getNegativeV();
Matrix3D View3D({ camera.getS(), camera.getU(), negativeV });
Matrix4D RotationV = View3D.convertTo4D();
Matrix4D ViewJ = camera.convertToView(View3D,Vec3D(-camera.getS().dot(camera.getEye()), -camera.getU().dot(camera.getEye()), camera.getV().dot(camera.getEye())));

Matrix4D ViewM = ViewJ.translate(-1.0f, 0.0f, 0.0f).mul(ViewJ);

////// CREATE SCENE GRAPH //////
SceneGraph createSceneGraph() {
    SceneGraph graph = SceneGraph(camera);

    Vec3D player1_piece_material_color = Vec3D(0.52f, 0.41f, 0.25f);
    Vec3D player2_piece_material_color = Vec3D(0.04f, 0.03f, 0.02f);
    Vec3D board_material_color = Vec3D(0.23f, 0.19f, 0.14f);

    Material piece_material = Material(black, WoodMaterial, WoodMaterial, shininess);
    Material board_material = Material(black, WoodMaterial, WoodMaterial, shininess);

    // Loading obj files and creating mesh
    std::string piece_mesh_fullname = "piece.obj";
    Mesh piece_mesh = Mesh(piece_mesh_fullname);

    std::string board_mesh_fullname = "board.obj";
    Mesh board_mesh = Mesh(board_mesh_fullname);
    
    // Loading shader files
    Vec3D pieces_painted_color = Vec3D(0.8f, 0.8f, 0.8f);
    Vec3D board_painted_color = Vec3D(0.5f, 0.5f, 0.5f);

    Shader piece_shader = Shader(&piece_mesh, eye, light, piece_material, pieces_painted_color);
    Shader board_shader = Shader(&board_mesh, eye, light, board_material, board_painted_color);

    //Board Node
    Matrix4D matrix_board = Matrix4D();
    SceneNode board = SceneNode(board_mesh, matrix_board, board_shader);
    graph.createNode(board);

    //Pieces Node
    Matrix4D matrix_pieces = Matrix4D() * Matrix4D::translate(5.25f, 0.0f, -5.25f);
    Shader new_shader_player1 = piece_shader;
    new_shader_player1.moveVertices(Vec3D(5.25f, 0.0f, -5.25f));
    SceneNode pieces = SceneNode(piece_mesh, matrix_pieces, new_shader_player1);

    //Player1
    SceneNode player1pieces = SceneNode(pieces.getMesh(), pieces.getMatrix(), pieces.getShaderProgram());
    
    //A child node for each piece for Player1
    Matrix4D matrix_each_piece_player1 = player1pieces.getMatrix();
    for (int i = 0; i < 12; i++) {
        Mesh player1_piece_mesh = player1pieces.getMesh();
        Shader player1_piece_shader = player1pieces.getShaderProgram();
        SceneNode each_piece_player1 = SceneNode(player1_piece_mesh, matrix_each_piece_player1, player1_piece_shader);
        if (i + 1 == 4) {
            matrix_each_piece_player1 = player1pieces.getMatrix() * Matrix4D::translate(-1.5f, 0.0f, 1.5f);
            player1_piece_shader = player1pieces.getShaderProgram();
            player1_piece_shader.moveVertices(Vec3D(-1.5f, 0.0f, 1.5f));
        }
        else if (i + 1 == 8) {
            matrix_each_piece_player1 = player1pieces.getMatrix() * Matrix4D::translate(0.0f, 0.0f, 3.0f);
            player1_piece_shader = player1pieces.getShaderProgram();
            player1_piece_shader.moveVertices(Vec3D(0.0f, 0.0f, 3.0f));
        }
        else {
            matrix_each_piece_player1 = matrix_each_piece_player1 * Matrix4D::translate(-3.0f, 0.0f, 0.0f);
            player1_piece_shader.moveVertices(Vec3D(-3.0f, 0.0f, 0.0f));
        }
        
        player1pieces.addChild(each_piece_player1);
    }
    
    pieces.addChild(player1pieces);

    //Player2
    Matrix4D matrix_pieces_player2 = pieces.getMatrix() * Matrix4D::translate(-10.5f, 0.0f, 10.5f);
    Vec3D pieces_painted_color_player2 = Vec3D(0.2f, 0.2f, 0.2f);
    Shader new_shader_player2 = pieces.getShaderProgram();
    new_shader_player2.moveVertices(Vec3D(-10.5f, 0.0f, 10.5f));
    new_shader_player2.setPaintedColor(pieces_painted_color_player2);

    SceneNode player2pieces = SceneNode(pieces.getMesh(), matrix_pieces_player2, new_shader_player2);

    //A child node for each piece for Player2
    Matrix4D matrix_each_piece_player2 = player2pieces.getMatrix();
    for (int i = 0; i < 12; i++) {
        Mesh player2_piece_mesh = player2pieces.getMesh();
        Shader player2_piece_shader = player2pieces.getShaderProgram();
        SceneNode each_piece_player2 = SceneNode(player2_piece_mesh, matrix_each_piece_player2, player2_piece_shader);
        if (i + 1 == 4) {
            matrix_each_piece_player2 = player2pieces.getMatrix() * Matrix4D::translate(1.5f, 0.0f, -1.5f);
            player2_piece_shader = player2pieces.getShaderProgram();
            player2_piece_shader.moveVertices(Vec3D(1.5f, 0.0f, -1.5f));

        }
        else if (i + 1 == 8) {
            matrix_each_piece_player2 = player2pieces.getMatrix() * Matrix4D::translate(0.0f, 0.0f, -3.0f);
            player2_piece_shader = player2pieces.getShaderProgram();
            player2_piece_shader.moveVertices(Vec3D(0.0f, 0.0f, -3.0f));
        }
        else {
            matrix_each_piece_player2 = matrix_each_piece_player2 * Matrix4D::translate(3.0f, 0.0f, 0.0f);
            player2_piece_shader.moveVertices(Vec3D(3.0f, 0.0f, 0.0f));
        }
        
        player2pieces.addChild(each_piece_player2);
    }

    pieces.addChild(player2pieces);
    
    //Add the pieces node to the graph
    graph.createNode(pieces);

    return graph;
}

SceneGraph sceneGraph = SceneGraph(camera);

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

    GLfloat* ViewMGL = ViewM.toGLfloats();
    glBindBuffer(GL_UNIFORM_BUFFER, VboId[1]);
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix), ViewMGL);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix), sizeof(Matrix), P);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Drawing directly in clip space

    animation();

    vector<SceneNode> thisNodes = sceneGraph.getNodes();
    Shader thisShader = thisNodes[0].getShaderProgram();

    if (!renderer) {
        renderer = new Renderer(VaoId, VboId, UniformId);
    }
    
    for (int i = 0; i < sceneGraph.getNodeNumber(); i++) {
        vector<SceneNode> all_nodes = sceneGraph.getNodes();
        SceneNode node = all_nodes[i];
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
                            renderer->drawTriangles(child2.getMesh().getVertices(), child2.getMesh().getVertexIdx(), child2.getMatrix(), child2.getShaderProgram());
                        else
                            renderer->drawTriangles(child2.getMesh().getVertices(), child2.getMesh().getVertexIdx(), child2.getMatrix(), child2.getShaderProgram());
                    }
                }
            }
        }
        else {
            //Board
            renderer->drawTriangles(node.getMesh().getVertices(), node.getMesh().getVertexIdx(), node.getMatrix(), node.getShaderProgram());
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
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    sceneGraph = createSceneGraph();
#ifdef ERROR_CALLBACK
    setupErrorCallback();
#endif
    createShaderProgram();
    return win;
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec)
{
    drawScene();
}

void saveImage(const char* filepath, GLFWwindow* w) {
    int width, height;
    glfwGetFramebufferSize(w, &width, &height);
    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
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
        if (key == GLFW_KEY_V) {
            time_t curr_time;
            tm* curr_tm;
            char time_string[100];

            time(&curr_time);
            curr_tm = localtime(&curr_time);

            string s = to_string(curr_tm->tm_year + 1900);
            s.append("-" + to_string(curr_tm->tm_mon + 1));
            s.append("-" + to_string(curr_tm->tm_mday));
            s.append(" " + to_string(curr_tm->tm_hour));
            s.append(";" + to_string(curr_tm->tm_min));
            s.append(";" + to_string(curr_tm->tm_sec) + ".png");
            saveImage(s.c_str(), window);

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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos;
    double ypos;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &xpos, &ypos);
        GLfloat pixels[4] = { 0 };
        glReadPixels((GLint)xpos, (GLint)ypos, 1, 1, GL_RGBA, GL_FLOAT, pixels);
        cout <<"Pixel read = " << pixels[0] << " " << pixels[1] << " " << pixels[2] << " " << pixels[3] << endl;
        GLfloat depth[4] = { 0 };
        glReadPixels((GLint)xpos, (GLint)ypos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, depth);

    }

    // Select the object & move it

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
        glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetKeyCallback(win, key_callback);
        glfwSetMouseButtonCallback(win, mouse_button_callback);
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
        640, 480, "Group 12", is_fullscreen, is_vsync);
    run(win);
    exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////