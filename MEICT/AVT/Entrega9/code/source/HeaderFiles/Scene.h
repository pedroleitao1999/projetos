#pragma once
#ifndef Scene_H
#define Scene_H

#include <iostream>
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include <stack>
#include <vector>

using namespace std;

class SceneNode {
private:
	Mesh mesh;
	Matrix4D matrix;
	Shader shader;
	vector<SceneNode> children;
	int childrenNumber;

public:
	SceneNode(Mesh mes, Matrix4D mat, Shader s);
	SceneNode(void);
	void setMesh(Mesh m) { mesh = m; };
	Mesh getMesh(void) { return mesh; };
	void setMatrix(Matrix4D m) { matrix = m; };
	Matrix4D getMatrix(void) { return matrix; };
	void setShaderProgram(Shader s) { shader = s; };
	Shader getShaderProgram(void) { return shader; };
	vector<SceneNode> getChildren(void) { return children; };
	int getChildrenNumber(void) { return childrenNumber; };
	void addChild(SceneNode c);
	vector<Vec4D> generateColors(float a, float b, float c);
};

class SceneGraph {
private:
	vector<SceneNode> nodes;
	Camera camera;
	int nodeNumber;

public:
	SceneGraph(Camera cam);
	void createNode(SceneNode n);
	void setCamera(Camera c) { camera = c; };
	Camera getCamera(void) { return camera; };
	vector<SceneNode> getNodes(void) { return nodes; }
	int getNodeNumber(void) { return nodeNumber; };
};
#endif // !Scene_H#pragma once
