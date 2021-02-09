#include <GL/glew.h>
#include "../HeaderFiles/Shader.h"

using namespace std;

Shader::Shader() {
	mesh = new Mesh();
	light = Light();
	material = Material();
}

// Shader constructor
Shader::Shader(Mesh* m, Vec3D e, Light l, Material mat, Vec3D pC) {
	mesh = m;
	eye = e;
	light = l;
	material = mat;
	painted_color = pC;
	vertices = mesh->getVertices();
	normals = mesh->getNormals();
	texCoords = mesh->getTexcoords();
}

void Shader::setMesh(Mesh* m) {
	mesh = m;
}

void Shader::setMaterial(Material mat) {
	material = mat;
}

void Shader::setPaintedColor(Vec3D pC) {
	painted_color = pC;
}

void Shader::moveVertices(Vec3D newPos) {
	for (int i = 0; i < vertices.size(); i++) {
		Vec4D oldVertex = vertices[i];
		vertices[i] = Vec4D(oldVertex.getX() + newPos.getX(), oldVertex.getY() + newPos.getY(), oldVertex.getZ() + newPos.getZ(), oldVertex.getW());
	}
}