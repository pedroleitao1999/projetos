#include <GL/glew.h>
#include "../HeaderFiles/Mesh.h"

using namespace std;

Texcoord::Texcoord(float a, float b) {
	u = a;
	v = b;
}

Texcoord::Texcoord() {
	u = 0.0;
	v = 0.0;
}

Mesh::Mesh(std::string& filename) {

	//Initialization
	vertices, vertexData, normals, normalData = vector<Vec4D>();
	texcoords, texcoordData = vector<Texcoord>();
	vertexIdx, texcoordIdx, normalIdx = vector<int>();
	numberVertices, numberTexcoords, numberNormals = 0;
	numberVertexData, numberTexcoordData, numberNormalData = 0;
	numberVertexIdx, numberTexcoordIdx, numberNormalIdx = 0;
	texcoordsLoaded, normalsLoaded = 0;

	//Load Mesh Data
	std::ifstream ifile(filename);
	std::string line;
	while (std::getline(ifile, line))
	{
		std::stringstream sline(line);
		parseLine(sline);
	}
	texcoordsLoaded = (numberTexcoordIdx > 0);
	normalsLoaded = (numberNormalIdx > 0);

	//Process Mesh Data
	for (unsigned int i = 0; i < (unsigned int)numberVertexIdx; i++) {
		unsigned int vi = vertexIdx[i];
		Vec4D v = vertexData[vi];
		vertices.resize((size_t)numberVertices + 1);
		vertices[numberVertices] = v;
		numberVertices = numberVertices + 1;
		if (texcoordsLoaded)
		{
			unsigned int ti = texcoordIdx[i];
			Texcoord t = texcoordData[ti];
			texcoords.resize((size_t)numberTexcoords + 1);
			texcoords[numberTexcoords] = t;
			numberTexcoords = numberTexcoords + 1;
		}
		if (normalsLoaded)
		{
			unsigned int ni = normalIdx[i];
			Vec4D n = normalData[ni];
			normals.resize((size_t)numberNormals + 1);
			normals[numberNormals] = n;
			numberNormals = numberNormals + 1;
		}
	}

}

Mesh::Mesh() {
	vertices, vertexData, normals, normalData = vector<Vec4D>();
	texcoords, texcoordData = vector<Texcoord>();
	vertexIdx, texcoordIdx, normalIdx = vector<int>();
	numberVertices, numberTexcoords, numberNormals = 0;
	numberVertexData, numberTexcoordData, numberNormalData = 0;
	numberVertexIdx, numberTexcoordIdx, numberNormalIdx = 0;
	texcoordsLoaded = 0;
	normalsLoaded = 0;
}

void Mesh::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}

void Mesh::parseVertex(std::stringstream& sin)
{
	float a, b, c;
	sin >> a >> b >> c;
	Vec4D v = Vec4D(a, b, c, 1.0);
	vertexData.resize((size_t)numberVertexData + 1);
	vertexData[numberVertexData] = v;
	numberVertexData = numberVertexData + 1;
}

void Mesh::parseTexcoord(std::stringstream& sin)
{
	float a, b;
	sin >> a >> b;
	Texcoord t = Texcoord(a, b);
	texcoordData.resize((size_t)numberTexcoordData + 1);
	texcoordData[numberTexcoordData] = t;
	numberTexcoordData = numberTexcoordData + 1;
}

void Mesh::parseNormal(std::stringstream& sin)
{
	float a, b, c;
	sin >> a >> b >> c;
	Vec4D n = Vec4D(a, b, c, 1.0);
	normalData.resize((size_t)numberNormalData + 1);
	normalData[numberNormalData] = n;
	numberNormalData = numberNormalData + 1;
}

void Mesh::parseFace(std::stringstream& sin)
{
	std::string token;
	if (numberNormalData == 0 && numberTexcoordData == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			sin >> token;
			vertexIdx.resize((size_t)numberVertexIdx + 1);
			vertexIdx[numberVertexIdx] = std::stoi(token) - 1;
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
				vertexIdx.resize((size_t)numberVertexIdx + 1);
				vertexIdx[numberVertexIdx] = std::stoi(token) - 1;
				numberVertexIdx = numberVertexIdx + 1;
			}
			std::getline(sin, token, '/');
			if (token.size() > 0)
			{
				texcoordIdx.resize((size_t)numberTexcoordIdx + 1);
				texcoordIdx[numberTexcoordIdx] = std::stoi(token) - 1;
				numberTexcoordIdx = numberTexcoordIdx + 1;
			}
			std::getline(sin, token, ' ');
			if (token.size() > 0)
			{
				normalIdx.resize((size_t)numberNormalIdx + 1);
				normalIdx[numberNormalIdx] = std::stoi(token) - 1;
				numberNormalIdx = numberNormalIdx + 1;
			}
		}
	}
}