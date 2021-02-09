//
// MESH CLASS
//

#include "main.h"

using namespace std;

////////////////////////////////////////////////////////////////////////// VECTOR
Vertex::Vertex() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vertex::Vertex(GLfloat a, GLfloat b, GLfloat c) {
	x = a;
	y = b;
	z = c;
}

////////////////////////////////////////////////////////////////////////// TEXCOORD
Texcoord::Texcoord() {
	u = 0.0f;
	v = 0.0f;
}

Texcoord::Texcoord(GLfloat a, GLfloat b) {
	u = a;
	v = b;
}

////////////////////////////////////////////////////////////////////////// NORMAL
Normal::Normal() {
	nx = 0.0f;
	ny = 0.0f;
	nz = 0.0f;
}

Normal::Normal(GLfloat a, GLfloat b, GLfloat c) {
	nx = a;
	ny = b;
	nz = c;
}

////////////////////////////////////////////////////////////////////////// MESH

Mesh::Mesh() {
	vertices = std::vector<Vertex>();
	texcoords = std::vector<Texcoord>();
	normals = std::vector<Normal>();
	vertexData = std::vector<Vertex>();
	texcoordData = std::vector<Texcoord>();
	normalData = std::vector<Normal>();
	vertexIdx = std::vector<int>();
	texcoordIdx = std::vector<int>();
	normalIdx = std::vector<int>();

	numberVertices = 0;
	numberTexcoords = 0;
	numberNormals = 0;
	numberVertexData = 0;
	numberTexcoordData = 0;
	numberNormalData = 0;
	numberVertexIdx = 0;
	numberTexcoordIdx = 0;
	numberNormalIdx = 0;
	texcoordsLoaded = 0;
	normalsLoaded = 0;
}

// Mesh Constructor
Mesh::Mesh(vector <Vertex> v, vector <Vertex> vD, vector<Texcoord> t, vector<Texcoord> tD, vector <Normal> n, vector <Normal> nD,
	vector<int> vI, vector<int> tI, vector<int> nI) {
	vertices = v;
	vertexData = vD;
	texcoords = t;
	texcoordData = tD;
	normals = n;
	normalData = nD;
	vertexIdx = vI;
	texcoordIdx = tI;
	normalIdx = nI;

	numberVertices = vertices.size();
	numberVertexData = vertexData.size();
	numberTexcoords = texcoords.size();
	numberTexcoordData = texcoordData.size();
	numberNormals = normals.size();
	numberNormalData = normalData.size();
	numberVertexIdx = vertexIdx.size();
	numberTexcoordIdx = texcoordIdx.size();
	numberNormalIdx = normalIdx.size();
	texcoordsLoaded = numberTexcoords > 0;
	normalsLoaded = numberNormals > 0;
}

////////////////////////////////////////////////////////////////////////// LOADER

Loader::Loader() {
	vertices = vector<Vertex>();
	texcoords = vector<Texcoord>();
	normals = vector<Normal>();
	vertexData = vector<Vertex>();
	texcoordData = vector<Texcoord>();
	normalData = vector<Normal>();
	vertexIdx = vector<int>();
	texcoordIdx = vector<int>();
	normalIdx = vector<int>();

	numberVertices = 0;
	numberTexcoords = 0;
	numberNormals = 0;
	numberVertexData = 0;
	numberTexcoordData = 0;
	numberNormalData = 0;
	numberVertexIdx = 0;
	numberTexcoordIdx = 0;
	numberNormalIdx = 0;
	texcoordsLoaded = 0;
	normalsLoaded = 0;
};

void Loader::parseVertex(std::stringstream& sin)
{
	float a, b, c;
	sin >> a >> b >> c;
	GLfloat x = (GLfloat)a;
	GLfloat y = (GLfloat)b;
	GLfloat z = (GLfloat)c;
	Vertex v = Vertex(x, y, z);
	vertexData.resize(numberVertexData + 1);
	vertexData[numberVertexData] = v;
	numberVertexData = numberVertexData + 1;
}

void Loader::parseTexcoord(std::stringstream& sin)
{
	GLfloat a, b;
	sin >> a >> b;
	Texcoord t = Texcoord(a, b);
	texcoordData.resize(numberTexcoordData + 1);
	texcoordData[numberTexcoordData] = t;
	numberTexcoordData = numberTexcoordData + 1;
}

void Loader::parseNormal(std::stringstream& sin)
{
	GLfloat a, b, c;
	sin >> a >> b >> c;
	Normal n = Normal(a, b, c);
	normalData.resize(numberNormalData + 1);
	normalData[numberNormalData] = n;
	numberNormalData = numberNormalData + 1;
}

void Loader::parseFace(std::stringstream& sin)
{
	std::string token;
	if (numberNormalData == 0 && numberTexcoordData == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			sin >> token;
			vertexIdx.resize(numberVertexIdx + 1);
			vertexIdx[numberVertexIdx] = std::stoi(token);
			numberVertexIdx = numberVertexIdx + 1;
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			std::getline(sin, token, '/');
			if (token.size() > 0)
			{
				vertexIdx.resize(numberVertexIdx + 1);
				vertexIdx[numberVertexIdx] = std::stoi(token);
				numberVertexIdx = numberVertexIdx + 1;
			}
			std::getline(sin, token, '/');
			if (token.size() > 0)
			{
				texcoordIdx.resize(numberTexcoordIdx + 1);
				texcoordIdx[numberTexcoordIdx] = std::stoi(token);
				numberTexcoordIdx = numberTexcoordIdx + 1;
			}
			std::getline(sin, token, ' ');
			if (token.size() > 0)
			{
				normalIdx.resize(numberNormalIdx + 1);
				normalIdx[numberNormalIdx] = std::stoi(token);
				numberNormalIdx = numberNormalIdx + 1;
			}
		}
	}
}

void Loader::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}

// Loading mesh by parsing files
void Loader::loadMeshData(std::string& filename)
{
	std::ifstream ifile(filename);
	std::string line;
	while (std::getline(ifile, line))
	{
		std::stringstream sline(line);
		parseLine(sline);
	}
	texcoordsLoaded = (numberTexcoordIdx > 0);
	normalsLoaded = (numberNormalIdx > 0);
}

void Loader::processMeshData()
{
	for (unsigned int i = 0; i < (unsigned int)numberVertexIdx; i++) {
		unsigned int vi = vertexIdx[i];
		Vertex v = vertexData[vi - 1];
		vertices.resize(numberVertices + 1);
		vertices[numberVertices] = v;
		numberVertices = numberVertices + 1;
		if (texcoordsLoaded)
		{
			unsigned int ti = texcoordIdx[i];
			Texcoord t = texcoordData[ti - 1];
			texcoords.resize(numberTexcoords + 1);
			texcoords[numberTexcoords] = t;
			numberTexcoords = numberTexcoords + 1;
		}
		if (normalsLoaded)
		{
			unsigned int ni = normalIdx[i];
			Normal n = normalData[ni - 1];
			normals.resize(numberNormals + 1);
			normals[numberNormals] = n;
			numberNormals = numberNormals + 1;
		}
	}
}

void Loader::freeMeshData()
{
	vertexData.clear();
	texcoordData.clear();
	normalData.clear();
	vertexIdx.clear();
	texcoordIdx.clear();
	normalIdx.clear();

	numberVertexData = 0;
	numberTexcoordData = 0;
	numberNormalData = 0;
	numberVertexIdx = 0;
	numberTexcoordIdx = 0;
	numberNormalIdx = 0;
}

Mesh* Loader::createMesh(std::string& filename)
{
	loadMeshData(filename);
	processMeshData();
	Mesh *mesh = new Mesh(vertices, vertexData, texcoords, texcoordData, normals, normalData, vertexIdx, texcoordIdx, normalIdx);
	freeMeshData();
	return mesh;
}