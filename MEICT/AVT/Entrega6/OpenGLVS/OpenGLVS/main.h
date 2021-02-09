#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <stack>
#include <cstdlib>
#include <math.h> 
#include <cassert>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <math.h> 
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEGREES_TO_RADIANS (GLfloat) 0.01745329251994329547
#define RADIANS_TO_DEGREES (GLfloat) 57.29577951308232185913

#define PI 3.14159265

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

#define COLORS 1

#define HEADER(X) std::cout<<std::endl<<(X)<<std::endl<<std::endl;

#ifndef DATE_H
#define DATE_H

using namespace std;

class Vector2d
{
public:
	void setX(const GLfloat a);
	void setY(const GLfloat b);
	const GLfloat getX(void);
	const GLfloat getY(void);
	Vector2d(GLfloat a, GLfloat b);
	void assign(Vector2d);
	void operator=(const Vector2d& vec);
	Vector2d operator+(const Vector2d& vec);
	Vector2d operator-(const Vector2d& vec);
	Vector2d operator*(const GLfloat k);
	static GLfloat dotProduct(Vector2d vec1, Vector2d vec2);
	bool operator==(const Vector2d& vec);
	bool operator!=(const Vector2d& vec);
	friend Vector2d operator*(const GLfloat k, const Vector2d& vec);
private:
	GLfloat x;
	GLfloat y;
};

class Vector3d
{
public:
	Vector3d();
	Vector3d(GLfloat a, GLfloat b, GLfloat c);
	void operator=(const Vector3d& vec);
	Vector3d operator+(const Vector3d& vec);
	Vector3d operator-(const Vector3d& vec);
	Vector3d operator*(const GLfloat k);
	static GLfloat dotProduct(Vector3d vec1, Vector3d vec2);
	Vector3d operator*(const Vector3d& vec);
	bool operator==(const Vector3d& vec);
	bool operator!=(const Vector3d& vec);
	void setX(GLfloat a);
	void setY(GLfloat b);
	void setZ(GLfloat c);
	GLfloat getX(void);
	GLfloat getY(void);
	GLfloat getZ(void);
	void assign(Vector3d);
	GLfloat distance(void);
	Vector3d normalize(void);
	static GLfloat quadruple(Vector3d a, Vector3d b, Vector3d c, Vector3d d);
	friend Vector3d operator*(const GLfloat k, const Vector3d& vec);
	std::string toString(void);

private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class Vector4d
{
public:
	Vector3d convertion(void);
	void setX(GLfloat a);
	void setY(GLfloat b);
	void setZ(GLfloat c);
	void setW(GLfloat d);
	GLfloat getX(void);
	GLfloat getY(void);
	GLfloat getZ(void);
	GLfloat getW(void);
	Vector4d(GLfloat a, GLfloat b, GLfloat c, GLfloat d);
	void assign(Vector4d);
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat w;
};


class Matrix2x2
{
public:
	Matrix2x2 operator+(const Matrix2x2& a);
	Matrix2x2 operator-(const Matrix2x2& a);
	Vector2d operator*(const Vector2d& vec);
	Matrix2x2 operator*(const Matrix2x2& a);
	Matrix2x2 operator*(const GLfloat k);
	Matrix2x2(GLfloat ux, GLfloat uy, GLfloat vx, GLfloat vy);
	Matrix2x2 transpose(void);
	Matrix2x2 inverse(void);
	GLfloat determinant(void);
	void setUx(GLfloat a) { ux = a; };
	void setUy(GLfloat b) { uy = b; };
	void setVx(GLfloat c) { vx = c; };
	void setVy(GLfloat d) { vy = d; };
	GLfloat getUx(void) { return ux; };
	GLfloat getUy(void) { return uy; };
	GLfloat getVx(void) { return vx; };
	GLfloat getVy(void) { return vy; };

private:
	GLfloat ux, uy;
	GLfloat vx, vy;
};

class Matrix4x4
{
public:
	void setUx(GLfloat a) { ux = a; };
	void setUy(GLfloat b) { uy = b; };
	void setUz(GLfloat c) { uz = c; };
	void setUn(GLfloat d) { un = d; };
	void setVx(GLfloat e) { vx = e; };
	void setVy(GLfloat f) { vy = f; };
	void setVz(GLfloat g) { vz = g; };
	void setVn(GLfloat h) { vn = h; };
	void setWx(GLfloat i) { wx = i; };
	void setWy(GLfloat j) { wy = j; };
	void setWz(GLfloat k) { wz = k; };
	void setWn(GLfloat l) { wn = l; };
	void setMx(GLfloat m) { mx = m; };
	void setMy(GLfloat n) { my = n; };
	void setMz(GLfloat o) { mz = o; };
	void setMn(GLfloat p) { mn = p; };
	GLfloat getUx(void) { return ux; };
	GLfloat getUy(void) { return uy; };
	GLfloat getUz(void) { return uz; };
	GLfloat getUn(void) { return un; };
	GLfloat getVx(void) { return vx; };
	GLfloat getVy(void) { return vy; };
	GLfloat getVz(void) { return vz; };
	GLfloat getVn(void) { return vn; };
	GLfloat getWx(void) { return wx; };
	GLfloat getWy(void) { return wy; };
	GLfloat getWz(void) { return wz; };
	GLfloat getWn(void) { return wn; };
	GLfloat getMx(void) { return mx; };
	GLfloat getMy(void) { return my; };
	GLfloat getMz(void) { return mz; };
	GLfloat getMn(void) { return mn; };
	Matrix4x4();
	Matrix4x4(GLfloat a, GLfloat b, GLfloat c, GLfloat d,
		GLfloat e, GLfloat f, GLfloat g, GLfloat h,
		GLfloat i, GLfloat j, GLfloat k, GLfloat l,
		GLfloat m, GLfloat n, GLfloat o, GLfloat p);
	Matrix4x4 operator*(const Matrix4x4& mat);
	Vector4d operator*(const Vector4d& vec);
	static Matrix4x4 translation(GLfloat a, GLfloat b, GLfloat c);
	static Matrix4x4 scale(GLfloat a, GLfloat b, GLfloat c);
	static Matrix4x4 rotationY(GLfloat angle);
	static Matrix4x4 perspective(GLfloat fov, GLfloat aspect, GLfloat n, GLfloat f);
	std::string toString(void);
	static Matrix4x4 generateViewRotation(Vector3d vVec, Vector3d sVec, Vector3d uVec);
	static Matrix4x4 createViewMatrix(Vector3d eyeVec, Vector3d vVec, Vector3d sVec, Vector3d uVec);
	static Matrix4x4 createOrthographicProjectionMatrix(GLfloat tlrGL, GLfloat tbtGL, GLfloat tnfGL, GLfloat slrGL, GLfloat sbtGL, GLfloat snfGL);

private:
	GLfloat ux, uy, uz, un;
	GLfloat vx, vy, vz, vn;
	GLfloat wx, wy, wz, wn;
	GLfloat mx, my, mz, mn;
};

class Matrix3x3
{
public:
	void setUx(GLfloat a) { ux = a; };
	void setUy(GLfloat b) { uy = b; };
	void setUz(GLfloat c) { uz = c; };
	void setVx(GLfloat d) { vx = d; };
	void setVy(GLfloat e) { vy = e; };
	void setVz(GLfloat f) { vz = f; };
	void setWx(GLfloat g) { wx = g; };
	void setWy(GLfloat h) { wy = h; };
	void setWz(GLfloat i) { wz = i; };
	GLfloat getUx(void) { return ux; };
	GLfloat getUy(void) { return uy; };
	GLfloat getUz(void) { return uz; };
	GLfloat getVx(void) { return vx; };
	GLfloat getVy(void) { return vy; };
	GLfloat getVz(void) { return vz; };
	GLfloat getWx(void) { return wx; };
	GLfloat getWy(void) { return wy; };
	GLfloat getWz(void) { return wz; };
	Matrix3x3 operator+(const Matrix3x3& a);
	Matrix3x3 operator-(const Matrix3x3& a);
	Matrix3x3 operator*(const Matrix3x3& a);
	Matrix3x3 operator*(const GLfloat k);
	Vector3d operator*(const Vector3d& vec);
	Matrix3x3(GLfloat a, GLfloat b, GLfloat c, GLfloat d, GLfloat e, GLfloat f, GLfloat g, GLfloat h, GLfloat i);
	Matrix3x3 transpose(void);
	Matrix3x3 inverse(void);
	GLfloat determinant(void);
	static Vector3d RRF(Vector3d v, Vector3d a, GLfloat angle);
	std::string toString(void);
	static Matrix3x3 rotationInX(GLfloat angle);
	static Matrix3x3 rotationInY(GLfloat angle);
	static Matrix3x3 rotationInZ(GLfloat angle);
	static Matrix3x3 eulerRotation(GLfloat yaw, GLfloat pitch, GLfloat roll);
	Matrix4x4 convertTo4x4(void);

private:
	GLfloat ux, uy, uz;
	GLfloat vx, vy, vz;
	GLfloat wx, wy, wz;
};

class ComplexNumber2d {
public:
	ComplexNumber2d(GLfloat a, GLfloat b);
	void setReal(const GLfloat a) { real = a; };
	void setImaginary(const GLfloat b) { imaginary = b; };
	GLfloat getReal(void) { return real; };
	GLfloat getImaginary(void) { return imaginary; };
	const ComplexNumber2d transpose(ComplexNumber2d& cn);
	bool operator==(const ComplexNumber2d& cn);
	bool isReal(void);
	ComplexNumber2d operator+(const ComplexNumber2d& cn);
	ComplexNumber2d operator*(const ComplexNumber2d& cn);
	GLfloat module(ComplexNumber2d cn);
	Matrix2x2 toMatrix(void);

private:
	GLfloat real;
	GLfloat imaginary;
};

class Quaternion {
public:
	Quaternion();
	Quaternion(GLfloat tQ, Vector3d vec);
	void setT(const GLfloat tQ) { t = tQ; };
	void setX(const GLfloat x) { v.setX(x); };
	void setY(const GLfloat y) { v.setY(y); };
	void setZ(const GLfloat z) { v.setZ(z); };
	GLfloat getScalarPart(void) { return t; };
	Vector3d getVectorPart(void) { return v; };
	bool isReal(void);
	bool isPure(void);
	friend Quaternion operator*(const GLfloat lambda, const Quaternion& q);
	Quaternion operator+(const Quaternion& q);
	Quaternion operator*(const Quaternion& q);
	static const Quaternion transpose(Quaternion& q);
	Quaternion dotProduct(Quaternion q0, Quaternion q1);
	static GLfloat module(Quaternion& q);
	Quaternion qFunction(void);
	Quaternion inverse(Quaternion& q);
	Matrix2x2 toMatrix2x2(void);
	Matrix4x4 toMatrix4x4(void);
	static Quaternion normalize(Quaternion& q);
	Vector3d getAxis(void);
	GLfloat getAngle(void);
	Matrix3x3 toMatrix3x3(void);
	static Quaternion lerp(GLfloat t, Quaternion q0, Quaternion q1);
	static Quaternion slerp(GLfloat t, Quaternion q0, Quaternion q1);
	static Quaternion toQuaternion(Vector3d axis, GLfloat angle);

private:
	GLfloat t;
	Vector3d v;
};

class Camera {
public:
	Camera(void);
	Vector3d getEye() { return eye; };
	Vector3d getCenter() { return center; };
	Vector3d getUp() { return up; };
	Vector3d getView() { return view; };
	Vector3d getSide() { return side; };
	Vector3d getV() { return v; };
	Vector3d getS() { return s; };
	Vector3d getU() { return u; };
	GLfloat getLeftProj() { return leftProj; };
	GLfloat getRightProj() { return rightProj; };
	GLfloat getBottom() { return bottom; };
	GLfloat getTop() { return top; };
	GLfloat getNear() { return near; };
	GLfloat getFar() { return far; };
	GLfloat getAngle() { return angle; };
	GLfloat getAspect() { return aspect; };
	GLfloat getTlr() { return tlr; };
	GLfloat getTbt() { return tbt; };
	GLfloat getTnf() { return tnf; };
	GLfloat getSlr() { return slr; };
	GLfloat getSbt() { return sbt; };
	GLfloat getSnf() { return snf; };
	Matrix4x4 getIdentityMatrix() { return identityMatrix; };
	Matrix4x4 getViewMatrix() { return viewMatrix; };
	Matrix4x4 getOrthographicProjectionMatrix() { return orthographicProjectionMatrix; };
	Matrix4x4 getPerspectiveProjectionMatrix() { return perspectiveProjectionMatrix; };
	void moveCamera(Vector3d newEye);
	void orientateCamera(Vector2d delta);
	void updateEye(Vector3d newEye);

private:
	Vector3d eye, center, up, view, side, v, s, u; //Elements for the View Matrix
	GLfloat leftProj, rightProj, bottom, top, near, far, angle, aspect, tlr, tbt, tnf, slr, sbt, snf; //Elements for the Projection Matrices
	Matrix4x4 identityMatrix; //Identity Matrix
	Matrix4x4 viewMatrix; //View Matrix
	Matrix4x4 orthographicProjectionMatrix, perspectiveProjectionMatrix; //Projection Matrices
};

class Vertex {
public:
	Vertex(void);
	Vertex(GLfloat a, GLfloat b, GLfloat c);
	GLfloat getX(void) { return x; };
	GLfloat getY(void) { return y; };
	GLfloat getZ(void) { return z; };

private:
	GLfloat x, y, z;
};

class Texcoord {
public:
	Texcoord(void);
	Texcoord(GLfloat a, GLfloat b);
	GLfloat getU(void) { return u; };
	GLfloat getV(void) { return v; };

private:
	GLfloat u, v;
};

class Normal {
public:
	Normal(void);
	Normal(GLfloat a, GLfloat b, GLfloat c);
	GLfloat getNx(void) { return nx; };
	GLfloat getNy(void) { return ny; };
	GLfloat getNz(void) { return nz; };

private:
	GLfloat nx, ny, nz;
};

class Mesh {
public:
	Mesh(void);
	Mesh(vector <Vertex> v, vector <Vertex> vD, vector<Texcoord> t, vector<Texcoord> tD, vector <Normal> n, vector <Normal> nD, 
		vector<int> vI, vector<int> tI, vector<int> nI);
	bool getTexcoordsLoaded(void) { return texcoordsLoaded; };
	bool getNormalsLoaded(void) { return normalsLoaded; };
	vector<Vertex> getVertices(void) { return vertices; };
	vector<Texcoord> getTexcoords(void) { return texcoords; };
	vector<Normal> getNormals(void) { return normals; };
	int getNumberVertices(void) { return numberVertices; };
	int getNumberTexcoords(void) { return numberTexcoords; };
	int getNumberNormals(void) { return numberNormals; };

private:
	//Loader loader;
	vector <Vertex> vertices, vertexData;
	vector<Texcoord> texcoords, texcoordData;
	vector <Normal> normals, normalData;
	vector<int> vertexIdx, texcoordIdx, normalIdx;
	int numberVertices, numberTexcoords, numberNormals;
	int numberVertexData, numberTexcoordData, numberNormalData;
	int numberVertexIdx, numberTexcoordIdx, numberNormalIdx;
	bool texcoordsLoaded, normalsLoaded;
};

class Loader {
public:
	Loader(void);
	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream& sin);
	void loadMeshData(std::string& filename);
	void processMeshData(void);
	void freeMeshData(void);
	Mesh* createMesh(std::string& filename);
	vector<Vertex> getVertices(void) { return vertices; };
	vector<Vertex> getVertexData(void) { return vertexData; };
	vector<Texcoord> getTexcoords(void) { return texcoords; };
	vector<Texcoord> getTexcoordData(void) { return texcoordData; };
	vector<Normal> getNormals(void) { return normals; };
	vector<Normal> getNormalData(void) { return normalData; };
	vector<int> getVertexIdx(void) { return vertexIdx; };
	vector<int> getTexcoordIdx(void) { return texcoordIdx; };
	vector<int> getNormalIdx(void) { return normalIdx; };
	int getNumberVertices(void) { return numberVertices; };
	int getNumberTexcoords(void) { return numberTexcoords; }
	int getNumberNormals(void) { return numberNormals; };
	int getNumberVertexData(void) { return numberVertexData; };
	int getNumberTexcoordData(void) { return numberTexcoordData; };
	int getNumberNormalData(void) { return numberNormalData; };
	int getNumberVertexIdx(void) { return numberVertexIdx; };
	int getNumberTexcoordIdx(void) { return numberTexcoordIdx; };
	int getNumberNormalIdx(void) { return numberNormalIdx; };
	bool getTexcoordsLoaded(void) { return texcoordsLoaded; };
	bool getNormalsLoaded(void) { return normalsLoaded; };

private:
	vector<Vertex> vertices, vertexData;
	vector<Texcoord> texcoords, texcoordData;
	vector<Normal> normals, normalData;
	vector<int> vertexIdx, texcoordIdx, normalIdx;
	int numberVertices, numberTexcoords, numberNormals;
	int numberVertexData, numberTexcoordData, numberNormalData;
	int numberVertexIdx, numberTexcoordIdx, numberNormalIdx;
	bool texcoordsLoaded, normalsLoaded;
};

class Shader {
public:
	Shader(void);
	Shader(Mesh* mesh);
	const std::string read(const std::string& filename);
	const GLuint checkCompilation(const GLuint shader_id, const std::string& filename);
	void checkLinkage(const GLuint program_id);
	const GLuint addShader(const GLuint program_id, const GLenum shader_type, const std::string& filename);
	void createShaderProgram(std::string& vs_file, std::string& fs_file);
	void destroyShaderProgram(void);
	GLuint getProgramId(void) { return ProgramId; };
	GLint getModelMatrix_UId(void) { return ModelMatrix_UId; };
	GLint getViewMatrix_UId(void) { return ViewMatrix_UId; };
	GLint getProjectionMatrix_UId(void) { return ProjectionMatrix_UId; };
	void setMesh(Mesh *m) { mesh = m; };

private:
	GLuint ProgramId;
	GLint ModelMatrix_UId, ViewMatrix_UId, ProjectionMatrix_UId;
	Mesh* mesh;
};

class SceneNode {
public:
	SceneNode(void);
	SceneNode(Mesh* mes, Matrix4x4 mat, Shader s);
	void setMesh(Mesh* m) { mesh = m; };
	Mesh* getMesh(void) { return mesh; };
	void setMatrix(Matrix4x4 m) { matrix = m; };
	Matrix4x4 getMatrix(void) { return matrix; };
	void setShaderProgram(Shader s) { shader = s; };
	Shader getShaderProgram(void) { return shader; };

private:
	Mesh* mesh;
	Matrix4x4 matrix;
	Shader shader;
};

class SceneGraph {
public:
	SceneGraph(void);
	void createNode(SceneNode n);
	void setCamera(Camera c) { camera = c; };
	Camera getCamera(void) { return camera; };
	vector<SceneNode> getNodes(void) { return nodes; }
	int getNodeNumber(void) { return nodeNumber; };
	void createBufferObjectsSceneGraph(GLuint VaoId);
	void drawSceneGraph(GLuint VaoId);
	
private:
	vector<SceneNode> nodes;
	Camera camera;
	int nodeNumber;
};

class Scene {
public:
	Scene(void);
	Scene(Mesh *mesh, Shader shader, GLuint VaoId);
	void createCubes(void);
	void destroyBufferObjects(GLuint VaoId);
	SceneGraph getSceneGraph(void) { return sceneGraph; };
	Camera getCamera(void) { return camera; };
	Mesh getMesh(void) { return *mesh; };
	void setMesh(Mesh *m) { mesh = m; };

private:
	Camera camera;
	Mesh* mesh;
	SceneGraph sceneGraph;
	GLuint vaoId;
	Shader shader;
};

#endif
