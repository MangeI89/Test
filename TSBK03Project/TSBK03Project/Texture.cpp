#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

//The texture constructor is written by BennyQBD
//https://github.com/BennyQBD/ModernOpenGLTutorial/blob/master/texture.cpp

Texture::Texture() {}

Texture::Texture(const string& fileName, const string name, unsigned int unit) {

	m_name = name;
	this->unit = unit;
	int width, height, numComponents;

	unsigned char* imageData = stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);

	if (imageData == NULL) {

		cerr << "Texture loading failed" << fileName << endl;

	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind(unsigned int unit, GLuint program) {

	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glUniform1i(glGetUniformLocation(program, m_name.c_str()), unit);
}

GLuint Texture::GetTexture() {
	return m_texture;
}

string Texture::GetTextureName() {
	return m_name;
}

unsigned int Texture::GetUnit() {
	return unit;
}
