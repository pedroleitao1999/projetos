//
// SHADER CLASS
//

#include "main.h"

using namespace std;
GLuint ProgramId;
GLint ModelMatrix_UId, ViewMatrix_UId, ProjectionMatrix_UId;

Shader::Shader() {
	ProgramId = 0;
	ModelMatrix_UId = 0;
	ViewMatrix_UId = 0;
	ProjectionMatrix_UId = 0;
	mesh = new Mesh();
}

// Shader constructor
Shader::Shader(Mesh* m) {
	ProgramId = 0;
	ModelMatrix_UId = 0;
	ViewMatrix_UId = 0;
	ProjectionMatrix_UId = 0;
	mesh = m;
}

const std::string Shader::read(const std::string& filename)
{
	std::ifstream ifile(filename);
	std::string shader_string, line;
	while (std::getline(ifile, line))
	{
		shader_string += line + "\n";
	}
	return shader_string;
}

const GLuint Shader::checkCompilation(const GLuint shader_id, const std::string& filename)
{
	GLint compiled;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
		GLchar* const log = new char[length];
		glGetShaderInfoLog(shader_id, length, &length, log);
		std::cerr << "[" << filename << "] " << std::endl << log;
		delete[] log;
		exit(EXIT_FAILURE);
	}
	return compiled;
}

void Shader::checkLinkage(const GLuint program_id) {
	GLint linked;
	glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
	if (linked == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
		GLchar* const log = new char[length];
		glGetProgramInfoLog(program_id, length, &length, log);
		std::cerr << "[LINK] " << std::endl << log << std::endl;
		delete[] log;
		exit(EXIT_FAILURE);
	}
}

const GLuint Shader::addShader(const GLuint program_id, const GLenum shader_type, const std::string& filename)
{
	const GLuint shader_id = glCreateShader(shader_type);
	const std::string scode = read(filename);
	const GLchar* code = scode.c_str();
	glShaderSource(shader_id, 1, &code, 0);
	glCompileShader(shader_id);
	checkCompilation(shader_id, filename);
	glAttachShader(program_id, shader_id);
	return shader_id;
}

void Shader::createShaderProgram(std::string& vs_file, std::string& fs_file)
{
	this->ProgramId = glCreateProgram();

	GLuint VertexShaderId = addShader(this->ProgramId, GL_VERTEX_SHADER, vs_file);
	GLuint FragmentShaderId = addShader(this->ProgramId, GL_FRAGMENT_SHADER, fs_file);

	glBindAttribLocation(ProgramId, VERTICES, "inPosition");
	if (mesh->getTexcoordsLoaded())
		glBindAttribLocation(ProgramId, TEXCOORDS, "inTexcoord");
	if (mesh->getNormalsLoaded())
		glBindAttribLocation(ProgramId, NORMALS, "inNormal");

	glLinkProgram(this->ProgramId);
	checkLinkage(this->ProgramId);

	glDetachShader(this->ProgramId, VertexShaderId);
	glDetachShader(this->ProgramId, FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteShader(FragmentShaderId);

	this->ModelMatrix_UId = glGetUniformLocation(this->ProgramId, "ModelMatrix");
	this->ViewMatrix_UId = glGetUniformLocation(this->ProgramId, "ViewMatrix");
	this->ProjectionMatrix_UId = glGetUniformLocation(this->ProgramId, "ProjectionMatrix");
}

void Shader::destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(this->ProgramId);
}