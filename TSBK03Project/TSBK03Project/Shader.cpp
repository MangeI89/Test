#include "shader.h"
#include <cassert>
#include <iostream>
#include <fstream>


//The following functions in this class is written by BennyQBD

//Shader::Shader(const string& fileName) 
//Shader::~Shader()
//string Shader::LoadShader(const string& fileName)
//void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage)
//GLuint Shader::CreateShader(const string& text, unsigned int type)
//https://github.com/BennyQBD/ModernOpenGLTutorial/blob/master/shader.cpp

Shader::Shader() {}

Shader::Shader(const string& fileName) {

	program = glCreateProgram();
	shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
		glAttachShader(program, shaders[i]);

	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "texCoord");
	glBindAttribLocation(program, 2, "normal");

	glLinkProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(program);
	CheckShaderError(program, GL_LINK_STATUS, true, "Invalid shader program");

	uniforms[MODEL] = glGetUniformLocation(program, "model");
	uniforms[CAMERA] = glGetUniformLocation(program, "camera");
	uniforms[PROJECTION] = glGetUniformLocation(program, "projection");

	//m_uniforms[0] = glGetUniformLocation(m_program, "MVP");
	//m_uniforms[1] = glGetUniformLocation(m_program, "Normal");
	//m_uniforms[2] = glGetUniformLocation(m_program, "lightDirection");
}

Shader::~Shader() {

	for (unsigned int i = 0; i < NUM_SHADERS; i++) {
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	glDeleteProgram(program);
}

void Shader::Bind() {

	glUseProgram(program);
}

GLuint Shader::GetProgram() {
	return program;
}

void Shader::LoadModelMatrix(const mat4 modelMatrix) {

	mat4 modelMatrix2 = modelMatrix;
	glUniformMatrix4fv(uniforms[MODEL], 1, GL_FALSE, &modelMatrix2[0][0]);
}

void Shader::LoadCameraMatrix(const mat4 cameraMatrix) {

	mat4 cameraMatrix2 = cameraMatrix;
	glUniformMatrix4fv(uniforms[CAMERA], 1, GL_FALSE, &cameraMatrix2[0][0]);
}

void Shader::LoadProjectionMatrix(const mat4& projection) {

	mat4 projectionMatrix = projection;
	glUniformMatrix4fv(uniforms[PROJECTION], 1, GL_FALSE, &projectionMatrix[0][0]);
}

void Shader::LoadLight(const vec3& position, const vec3& color, const GLfloat& exponent, const string& name) {
	string pName = name + "Position";
	string cName = name + "Color";
	string eName = name + "Exponent";

	glUniform3f(glGetUniformLocation(program, pName.c_str()), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(program, cName.c_str()), color.x, color.y, color.z);
	glUniform1f(glGetUniformLocation(program, eName.c_str()), exponent);
}

void Shader::SetLightOnOff(const bool& value, const string& name) {
	string ooName = name + "OnOff";
	glUniform1i(glGetUniformLocation(program, ooName.c_str()), value);
}

void Shader::LoadDayNightBlendFactor(const float& dayNightBlend) {
	glUniform1f(glGetUniformLocation(program, "dayNightBlend"), dayNightBlend);
}

void Shader::LoadBoolean(const bool& value, const string& name) {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::LoadVec2(const vec2& vector, const string& name) {
	glUniform2f(glGetUniformLocation(program, name.c_str()), vector.x, vector.y);
}

void Shader::LoadVec3(const vec3& vector, const string& name) {
	glUniform3f(glGetUniformLocation(program, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::LoadVec4(const vec4& vector, const string& name) {
	glUniform4f(glGetUniformLocation(program, name.c_str()), vector.x, vector.y, vector.z, vector.w);
}

void Shader::LoadMat4(const mat4& matrix, const string& name) {
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::LoadFloat(const float& value, const string& name) {
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::LoadUnsignedInt(const GLuint& value, const string& name) {
	glUniform1ui(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::LoadClipPlane(const vec4& vector) {
	glUniform4f(glGetUniformLocation(program, "clipPlane"), vector.x, vector.y, vector.z, vector.w);
}

void Shader::LoadStaticTexture(const shared_ptr<Texture>& texture) {
	glActiveTexture(GL_TEXTURE0 + texture->GetUnit());
	glBindTexture(GL_TEXTURE_2D, texture->GetTexture());
	glUniform1i(glGetUniformLocation(program, texture->GetTextureName().c_str()), texture->GetUnit());
}

void Shader::LoadFBOTexture(const shared_ptr<FrameBufferObject>& fbo) {
	glActiveTexture(GL_TEXTURE0 + fbo->GetUnit());
	glBindTexture(GL_TEXTURE_2D, fbo->GetTexture());
	glUniform1i(glGetUniformLocation(program, fbo->GetTextureName().c_str()), fbo->GetUnit());
}

string Shader::LoadShader(const string& fileName) {

	ifstream file;
	file.open((fileName).c_str());

	string output;
	string line;

	if (file.is_open()) {
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else {
		cerr << "Unable to load shader: " << fileName << endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		cerr << errorMessage << ": '" << error << "'" << endl;
	}
}

GLuint Shader::CreateShader(const string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		cerr << "Error compiling shader type " << type << endl;

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}