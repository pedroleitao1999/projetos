#pragma once
#ifndef Mesh_H
#define Mesh_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "Vec3D.h"
#include "Vec4D.h"
#include <vector>
#include <array>

using namespace std;

class Texcoord {
private:
	float u, v;

public:
	Texcoord(void);
	Texcoord(float a, float b);
	GLfloat getU(void) { return u; };
	GLfloat getV(void) { return v; };
	void setU(float a) { u = a; };
	void setV(float b) { v = b; };
};

class Mesh {
private:
	vector<Vec4D> vertices, vertexData;
	vector<Texcoord> texcoords, texcoordData;
	vector<Vec4D> normals, normalData;
	vector<int> vertexIdx, texcoordIdx, normalIdx;
	int numberVertices, numberTexcoords, numberNormals;
	int numberVertexData, numberTexcoordData, numberNormalData;
	int numberVertexIdx, numberTexcoordIdx, numberNormalIdx;
	bool texcoordsLoaded, normalsLoaded;
	GLuint VaoId, VboId[3];

public:
	Mesh(void);
	Mesh(std::string& filename);
	bool getTexcoordsLoaded(void) { return texcoordsLoaded; };
	bool getNormalsLoaded(void) { return normalsLoaded; };
	vector<Vec4D> getVertices(void) { return vertices; };
	vector<Texcoord> getTexcoords(void) { return texcoords; };
	vector<Vec4D> getNormals(void) { return normals; };
	vector<int> getVertexIdx(void) { return vertexIdx; };
	vector<int> getTexcoordIdx(void) { return texcoordIdx; };
	vector<int> getNormalIdx(void) { return normalIdx; };
	int getNumberVertices(void) { return numberVertices; };
	int getNumberTexcoords(void) { return numberTexcoords; };
	int getNumberNormals(void) { return numberNormals; };
	void parseLine(std::stringstream& sin);
	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	GLuint getVaoId(void) { return VaoId; };
	GLuint* getVboId(void) { return VboId; };
};

#endif // !Mesh_H#pragma once#pragma once
