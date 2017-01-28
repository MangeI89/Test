#pragma once
#include <string>
#include <GL/glew.h>

using namespace std;

class Texture
{
public:
	Texture();
	Texture(const string& fileName, const string name, unsigned int unit);

	GLuint GetTexture();
	string GetTextureName();

	void Bind(unsigned int unit, GLuint program);
	unsigned int GetUnit();
	~Texture();

private: 
	GLuint m_texture;
	string m_name;
	unsigned int unit;
};

