#pragma once
#include <GL/glew.h>
#include <iostream>

using namespace std;

class FrameBufferObject
{
public:
	FrameBufferObject(const GLuint& unit, const string& name, const GLuint& width, const GLuint& height);
	FrameBufferObject(const GLuint& unit, const string& name, const GLuint& width, const GLuint& height, const string& test);

	void Bind();
	void Bind2();
	void BindTexture(GLuint program);
	GLuint GetTexture();
	string GetTextureName();
	unsigned int GetUnit();
	void Unbind();

	~FrameBufferObject();

private:
	GLuint fbo;
	GLuint rbo;
	GLuint texture;
	GLuint texture2;
	GLuint unit;
	string name;
	GLuint width;
	GLuint height;
};

