#pragma once
#ifndef Shader_H
#define Shader_H

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

#include <iostream>
#include "Mesh.h"
#include <vector>

using namespace std;

class Shader {
private:
	GLuint ProgramIdShader;
	GLint ModelMatrix_UId, ViewMatrix_UId, ProjectionMatrix_UId;
	Mesh* mesh;

public:
	Shader(void);
	Shader(Mesh* mesh);
	const std::string read(const std::string& filename);
	const GLuint checkCompilation(const GLuint shader_id, const std::string& filename);
	void checkLinkage(const GLuint program_id);
	const GLuint addShader(const GLuint program_id, const GLenum shader_type, const std::string& filename);
	void createShaderProgram(std::string& vs_file, std::string& fs_file);
	void destroyShaderProgram(void);
	GLuint getProgramId(void) { return ProgramIdShader; };
	GLint getModelMatrix_UId(void) { return ModelMatrix_UId; };
	GLint getViewMatrix_UId(void) { return ViewMatrix_UId; };
	GLint getProjectionMatrix_UId(void) { return ProjectionMatrix_UId; };
	void setMesh(Mesh* m) { mesh = m; };
};
#endif // !Shader_H#pragma once
