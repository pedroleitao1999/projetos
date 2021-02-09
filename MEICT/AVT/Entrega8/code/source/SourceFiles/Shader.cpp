#include <GL/glew.h>
#include "../HeaderFiles/Shader.h"

using namespace std;
GLuint ProgramIdShader;
GLint ModelMatrix_UId, ViewMatrix_UId, ProjectionMatrix_UId;

Shader::Shader() {
	ProgramIdShader = 0;
	ModelMatrix_UId = 0;
	ViewMatrix_UId = 0;
	ProjectionMatrix_UId = 0;
	mesh = new Mesh();
}

// Shader constructor
Shader::Shader(Mesh* m) {
	ProgramIdShader = 0;
	ModelMatrix_UId = 0;
	ViewMatrix_UId = 0;
	ProjectionMatrix_UId = 0;
	mesh = m;
}