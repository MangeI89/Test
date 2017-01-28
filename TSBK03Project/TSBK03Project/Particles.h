#pragma once
#include <gl\glew.h>
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>
#include <memory>
#include "Mesh.h"
#include "Particle.h"
#include "Shader.h"
#include "Light.h"

using namespace std;
using namespace glm;

class Particles
{
public:
	Particles(vector<vec2> quad, vector<vec2> quadTexCoords);
	~Particles();
	void GenerateParticles(const vec3& position, const vec3& velocity, const vec3& velRandomness, const GLuint nrOfParticles);
	void GenerateParticles2(const vec3& position, const vec3& velocity, const vec3& velRandomness, const GLuint nrOfParticles);
	void InitBufferData();
	void InitBuffers();
	void UpdateBuffers(const vec3& cameraForward, const float& deltaT, shared_ptr<Light> light);
	void ToggleAnimating();
	void ToggleRendering();
	void SetAnimating(bool isAnimating);
	void SetRendering(bool isRendering);
	bool IsAnimating();
	bool IsRendering();
	void Reset();
	void Draw();
	void AttachShader(const shared_ptr<Shader>& shader);
	shared_ptr<Shader> GetShader();

private:
	GLuint nrOfParticles;
	GLuint quadPositionVBO, quadTexCoordVBO;
	GLuint quadVAO, quadVBO, modelMatrixVBO, offsetVBO, blendFactorVBO;
	vector<Particle> particles;
	vector<mat4> modelMatrices;
	vector<vec2> offsets;
	vector<GLfloat> blendFactors;
	shared_ptr<Shader> shader;
	bool isAnimating;
	bool isRendering;
};

