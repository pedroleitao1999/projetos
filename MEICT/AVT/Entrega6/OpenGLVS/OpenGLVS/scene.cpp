//
// SCENE CLASS
//

#include "main.h"

using namespace std;
stack<Matrix4x4> MatrixStack;
typedef GLfloat Matrix[16];
const GLuint UBO_BP = 0;
GLuint VboVertices, VboTexcoords, VboNormals;

//Camera
Camera cam = Camera();

////////////////////////////////////////////////////////////////////////// MATRICES

// View Matrix
Matrix4x4 ViewMatrix = cam.getViewMatrix();
vector<GLfloat> VIEW = {
	ViewMatrix.getUx(), ViewMatrix.getVx(), ViewMatrix.getWx(), ViewMatrix.getMx(),
	ViewMatrix.getUy(), ViewMatrix.getVy(), ViewMatrix.getWy(), ViewMatrix.getMy(),
	ViewMatrix.getUz(), ViewMatrix.getVz(), ViewMatrix.getWz(), ViewMatrix.getMz(),
	ViewMatrix.getUn(), ViewMatrix.getVn(), ViewMatrix.getWn(), ViewMatrix.getMn()
};

// Orthographic Projection Matrix
Matrix4x4 OrthographicMatrix = cam.getOrthographicProjectionMatrix();
vector<GLfloat> ORTHOGRAPHIC_PROJECTION = {
	OrthographicMatrix.getUx(), OrthographicMatrix.getVx(), OrthographicMatrix.getWx(), OrthographicMatrix.getMx(),
	OrthographicMatrix.getUy(), OrthographicMatrix.getVy(), OrthographicMatrix.getWy(), OrthographicMatrix.getMy(),
	OrthographicMatrix.getUz(), OrthographicMatrix.getVz(), OrthographicMatrix.getWz(), OrthographicMatrix.getMz(),
	OrthographicMatrix.getUn(), OrthographicMatrix.getVn(), OrthographicMatrix.getWn(), OrthographicMatrix.getMn()
};

// Perspective Projection Matrix
Matrix4x4 PerspectiveMatrix = cam.getPerspectiveProjectionMatrix();
Matrix PERSPECTIVE_PROJECTION = {
	PerspectiveMatrix.getUx(), PerspectiveMatrix.getVx(), PerspectiveMatrix.getWx(), PerspectiveMatrix.getMx(),
	PerspectiveMatrix.getUy(), PerspectiveMatrix.getVy(), PerspectiveMatrix.getWy(), PerspectiveMatrix.getMy(),
	PerspectiveMatrix.getUz(), PerspectiveMatrix.getVz(), PerspectiveMatrix.getWz(), PerspectiveMatrix.getMz(),
	PerspectiveMatrix.getUn(), PerspectiveMatrix.getVn(), PerspectiveMatrix.getWn(), PerspectiveMatrix.getMn()
};


////////////////////////////////////////////////////////////////////////// SCENEGRAPH

// Scenegraph constructor
SceneGraph::SceneGraph() {
	nodes = vector<SceneNode>();
	nodeNumber = 0;
}

// Creating Buffer Objects
void SceneGraph::createBufferObjectsSceneGraph(GLuint VaoId) {

	int number = nodeNumber;

	// For each existing node
	for (int i = 0; i < number; i++) {

		SceneNode node = nodes[i];
		vector<Vertex> vertices = node.getMesh()->getVertices();
		GLfloat verticesArray[108];
		int j = 0;
		for (int k = 0; k < (vertices.size() * 3); k += 3) {

			verticesArray[k] = vertices[j].getX();
			verticesArray[k + 1] = vertices[j].getY();
			verticesArray[k + 2] = vertices[j].getZ();
			j++;
		}

		int numVertices = node.getMesh()->getNumberVertices();

		glGenVertexArrays(1, &VaoId);
		glBindVertexArray(VaoId);
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(GLfloat) * 3, 
			verticesArray, GL_STREAM_DRAW);

		
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VERTICES);

		// If the node has texcoords
		if (node.getMesh()->getTexcoordsLoaded()) {
				
			vector<Texcoord> texcoords = node.getMesh()->getTexcoords();
			GLfloat texcoordsArray[72];
			int j = 0;
			for (int k = 0; k < texcoords.size() * 2; k += 2) {

				texcoordsArray[k] = texcoords[j].getU();
				texcoordsArray[k + 1] = texcoords[j].getV();
				j++;
			}
			int numTexcoords = node.getMesh()->getNumberTexcoords();

			glBindVertexArray(VaoId);
			glGenBuffers(1, &VboTexcoords);
			glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
			glBufferData(GL_ARRAY_BUFFER, numTexcoords * sizeof(GLfloat) * 2,
				texcoordsArray, GL_STREAM_DRAW);

			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(TEXCOORDS);

				
		}

		// If the node has normals
		if (node.getMesh()->getNormalsLoaded()) {
				
			vector<Normal> normals = node.getMesh()->getNormals();
			GLfloat normalsArray[108];
			int j = 0;
			for (int k = 0; k < normals.size() * 3; k += 3) {

				normalsArray[k] = normals[j].getNx();
				normalsArray[k + 1] = normals[j].getNy();
				normalsArray[k + 2] = normals[j].getNz();
				j++;
			}
			int numNormals = node.getMesh()->getNumberNormals();

			glBindVertexArray(VaoId);
			glGenBuffers(1, &VboNormals);
			glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
			glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(GLfloat) * 3,
				normalsArray, GL_STREAM_DRAW);

			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(NORMALS);

		}

		glBindVertexArray(0);

	}

}

// Drawing scene
void SceneGraph::drawSceneGraph(GLuint VaoId) {

	int number = nodeNumber;

	// For each existing node draw the respective object
	for (int i = 0; i < number; i++) {

		SceneNode node = nodes[i];
		vector<Vertex> vertices = node.getMesh()->getVertices();
		GLfloat verticesArray[108];
		int j = 0;
		for (int k = 0; k < vertices.size() * 3; k+=3) {

			verticesArray[k] = vertices[j].getX();
			verticesArray[k + 1] = vertices[j].getY();
			verticesArray[k + 2] = vertices[j].getZ();
			j++;
		}
		int numVertices = node.getMesh()->getNumberVertices();

		Matrix4x4 matModel = node.getMatrix();
		vector<GLfloat> MODEL = {
			matModel.getUx(), matModel.getVx(), matModel.getWx(), matModel.getMx(),
			matModel.getUy(), matModel.getVy(), matModel.getWy(), matModel.getMy(),
			matModel.getUz(), matModel.getVz(), matModel.getWz(), matModel.getMz(),
			matModel.getUn(), matModel.getVn(), matModel.getWn(), matModel.getMn()
		};

		Shader shader = node.getShaderProgram();
		glBindVertexArray(VaoId);
		glUseProgram(shader.getProgramId());
		
		glUniformMatrix4fv(shader.getModelMatrix_UId(), 1, GL_TRUE, MODEL.data());
		glUniformMatrix4fv(shader.getViewMatrix_UId(), 1, GL_TRUE, VIEW.data());
		glUniformMatrix4fv(shader.getProjectionMatrix_UId(), 1, GL_TRUE, ORTHOGRAPHIC_PROJECTION.data());
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);

		glBindVertexArray(0);
		glUseProgram(0);

	}

}

void SceneGraph::createNode(SceneNode n) {
	nodes.resize(nodeNumber + 1);
	nodes[nodeNumber] = n;
	nodeNumber = nodeNumber + 1;
}

////////////////////////////////////////////////////////////////////////// SCENENODE

//////// SceneNode ////////
SceneNode::SceneNode() {
	mesh = new Mesh();
}

// SceneNode constructor
SceneNode::SceneNode(Mesh* mes, Matrix4x4 mat, Shader s) {
	mesh = mes;
	matrix = mat;
	shader = s;
}

////////////////////////////////////////////////////////////////////////// SCENE

// Scene constructor
Scene::Scene(Mesh* m, Shader s, GLuint vaoi) {
	mesh = m;
	camera = cam;
	vaoId = vaoi;
	shader = s;
	sceneGraph = SceneGraph();
}

Scene::Scene() {
	mesh = new Mesh();
	camera = cam;
	vaoId = 0;
	shader = Shader();
	sceneGraph = SceneGraph();
}

void Scene::createCubes() {

	Matrix4x4 tx = Matrix4x4::translation(1.0f, 0.0f, 0.0f);
	Matrix4x4 ty = Matrix4x4::translation(0.0f, 1.0f, 0.0f);
	Matrix4x4 tz = Matrix4x4::translation(0.0f, 0.0f, 1.0f);
	SceneNode cube1 = SceneNode(mesh, camera.getIdentityMatrix(), shader);
	sceneGraph.createNode(cube1);
	MatrixStack.push(camera.getIdentityMatrix());

	Matrix4x4 t1 = tz * MatrixStack.top();
	MatrixStack.pop();
	SceneNode cube2 = SceneNode(mesh, t1, shader);
	sceneGraph.createNode(cube2);
	MatrixStack.push(t1);

	Matrix4x4 t2 = tz * MatrixStack.top();
	MatrixStack.pop();
	SceneNode cube3 = SceneNode(mesh, t2, shader);
	sceneGraph.createNode(cube3);
	MatrixStack.push(t2);

	Matrix4x4 t3 = tz * MatrixStack.top();
	MatrixStack.pop();
	SceneNode cube4 = SceneNode(mesh, t3, shader);
	sceneGraph.createNode(cube4);
	MatrixStack.push(t3);

	Matrix4x4 t4 = ty * MatrixStack.top();
	MatrixStack.pop();
	SceneNode cube5 = SceneNode(mesh, t4, shader);
	sceneGraph.createNode(cube5);
	MatrixStack.push(t4);

	Matrix4x4 t5 = ty * MatrixStack.top();
	MatrixStack.pop();
	SceneNode cube6 = SceneNode(mesh, t5, shader);
	sceneGraph.createNode(cube6);
	MatrixStack.push(t5);

	Matrix4x4 t6 = ty * MatrixStack.top();
	MatrixStack.pop();
	SceneNode cube7 = SceneNode(mesh, t6, shader);
	sceneGraph.createNode(cube7);
	MatrixStack.push(t6);

	Matrix4x4 t7 = tx * MatrixStack.top();
	MatrixStack.pop();
	SceneNode cube8 = SceneNode(mesh, t7, shader);
	sceneGraph.createNode(cube8);
	MatrixStack.push(t7);

	Matrix4x4 t8 = tx * MatrixStack.top();
	MatrixStack.pop();
	SceneNode cube9 = SceneNode(mesh, t8, shader);
	sceneGraph.createNode(cube9);
}


void Scene::destroyBufferObjects(GLuint VaoId)
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}