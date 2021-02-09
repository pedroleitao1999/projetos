#pragma once
#ifndef Shader_H
#define Shader_H

#include <iostream>
#include "Mesh.h"
#include "Light.h"
#include "Material.h"
#include <vector>

using namespace std;

class Shader {
private:
	Mesh* mesh;
	Vec3D eye;
	Light light;
	Material material;
	Vec3D painted_color;
	vector<Vec4D> vertices, normals;
	vector<Texcoord> texCoords;
	vector<Vec4D> colors;

public:
	Shader(void);
	Shader(Mesh* mesh, Vec3D e, Light l, Material mat, Vec3D pC);
	Light getLight(void) { return light; };
	Material getMaterial(void) { return material; };
	Vec3D getPaintedColor(void) { return painted_color; };
	vector<Vec4D> getVertices(void) { return vertices; };
	vector<Vec4D> getNormals(void) { return normals; };
	vector<Texcoord> getTexCoords(void) { return texCoords; };
	vector<Vec4D> getColors(void) { return colors; };
	Mesh* getMesh(void) { return mesh; };
	void setMesh(Mesh* m);
	void setMaterial(Material mat);
	void setPaintedColor(Vec3D pC);
	void moveVertices(Vec3D newPos);
	float getLightPower(void) { return light.getPower(); };
	float getShininess(void) { return material.getShininess(); };
};
#endif // !Shader_H#pragma once
