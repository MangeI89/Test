#pragma once
#include <string>
#include <GL/glew.h>
#include <vector>
#include <memory>
#include "Texture.h"
#include "Camera.h"
#include "Projection.h"
#include "FrameBufferObject.h"

using namespace std;
using namespace glm;

class Shader
{
public:

	Shader();

	Shader(const string& fileName);

	void Bind();
	
	void LoadModelMatrix(const mat4 modelMatrix);
	void LoadCameraMatrix(const mat4 cameraMatrix);
	void LoadProjectionMatrix(const mat4& projection);

	void LoadLight(const vec3& position, const vec3& color, const GLfloat& exponent, const string& name);
	void SetLightOnOff(const bool& value, const string& name);
	void LoadDayNightBlendFactor(const float& dayNightBlend);

	void LoadBoolean(const bool& value, const string& name);
	void LoadVec2(const vec2& vector, const string& name);
	void LoadVec3(const vec3& vector, const string& name);
	void LoadVec4(const vec4& vector, const string& name);

	void LoadMat4(const mat4& matrix, const string& name);

	void LoadFloat(const float& value, const string& name);
	void LoadUnsignedInt(const GLuint& value, const string& name);

	void LoadClipPlane(const vec4& vector);

	void LoadStaticTexture(const shared_ptr<Texture>& texture);
	void LoadFBOTexture(const shared_ptr<FrameBufferObject>& fbo);
	GLuint GetProgram();

	virtual ~Shader();
protected:
private:

	static const unsigned int NUM_SHADERS = 2;
	static const unsigned int NUM_UNIFORMS = 3;

	string LoadShader(const string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const string& errorMessage);
	GLuint CreateShader(const string& text, unsigned int type);

	enum {
		MODEL,
		CAMERA,
		PROJECTION
	};

	GLuint program;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[3];
};