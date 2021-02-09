#include <GL/glew.h>
#include "../HeaderFiles/Scene.h"

using namespace std;
stack<Matrix4D> MatrixStack;
typedef GLfloat Matrix[16];
const GLuint UBO_BP = 0;
GLuint VboVertices, VboTexcoords, VboNormals;

//////// SceneGraph ////////
SceneGraph::SceneGraph(Camera cam) {
	nodes = vector<SceneNode>();
	camera = cam;
	nodeNumber = 0;
}

void SceneGraph::createNode(SceneNode n) {
	int new_nodeNumber = nodeNumber + 1;
	nodes.resize((size_t)new_nodeNumber);
	nodes[nodeNumber] = n;
	nodeNumber = nodeNumber + 1;
}

//////// SceneNode ////////
SceneNode::SceneNode(Mesh mes, Matrix4D mat, Shader s) {
	mesh = mes;
	matrix = mat;
	shader = s;
	children = vector<SceneNode>();
	childrenNumber = 0;
}

SceneNode::SceneNode() {
	children = vector<SceneNode>();
	childrenNumber = 0;
}

void SceneNode::addChild(SceneNode c) {
	int new_childrenNumber = childrenNumber + 1;
	children.resize((size_t)new_childrenNumber);
	children[childrenNumber] = c;
	childrenNumber = childrenNumber + 1;
}

vector<Vec4D> SceneNode::generateColors(float a, float b, float c) {
	vector<Vec4D> all_colors = vector<Vec4D>();
	Vec4D color = Vec4D(a, b, c, 1.0);
	for (int i = 0; i < mesh.getNumberVertices(); i++) {
		all_colors.resize((size_t)i+1);
		all_colors[i] = color;
	}
	return all_colors;
}



