#include "Particles.h"


Particles::Particles(vector<vec2> quadVertices, vector<vec2> quadTexCoords) {
	
	this->nrOfParticles = 0;

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1, &quadPositionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadPositionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices[0])*6, &quadVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &quadTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadTexCoords[0]) * 6, &quadTexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

Particles::~Particles() { }

void Particles::GenerateParticles(const vec3& position, const vec3& velocity, const vec3& velRandomness, const GLuint nrOfParticles) {

	mat4 modelMatrix = mat4(1.0);
	vec2 offset = vec2(0, 0);
	float blend = 0;

	isAnimating = false;
	isRendering = false;

	for (int i = this->nrOfParticles; i < this->nrOfParticles + nrOfParticles; i++) {

		vec3 randomVelocity;

		float fx = (float)rand() / RAND_MAX;
		float fy = (float)rand() / RAND_MAX;
		float fz = (float)rand() / RAND_MAX;

		float yOffset = 2 * fy * velRandomness.y;
		randomVelocity.x = -velRandomness.x + 2 * fx * velRandomness.x;
		randomVelocity.y = 5.0 + yOffset;
		randomVelocity.z = -velRandomness.z + 2 * fz * velRandomness.z;
		
		Particle particle(vec3(position.x, position.y + yOffset, position.z), velocity + randomVelocity);
		particles.push_back(particle);

		modelMatrices.push_back(modelMatrix);
		offsets.push_back(offset);
		blendFactors.push_back(blend);
	}

	this->nrOfParticles += nrOfParticles;
}

void Particles::GenerateParticles2(const vec3& position, const vec3& velocity, const vec3& velRandomness, const GLuint nrOfParticles) {

	mat4 modelMatrix = mat4(1.0);
	vec2 offset = vec2(0, 0);
	float blend = 0;

	isAnimating = false;
	isRendering = false;

	for (int i = this->nrOfParticles; i < this->nrOfParticles + nrOfParticles; i++) {

		vec3 randomVelocity;

		float fx = (float)rand() / RAND_MAX;
		float fy = (float)rand() / RAND_MAX;
		float fz = (float)rand() / RAND_MAX;

		float yOffset = 2 * fy * velRandomness.y;
		randomVelocity.x = -velRandomness.x + 2 * fx * velRandomness.x;
		randomVelocity.y = 5.0 + yOffset;
		randomVelocity.z = -velRandomness.z + 2 * fz * velRandomness.z;

		Particle particle(vec3(position.x, position.y + yOffset, position.z), velocity + randomVelocity);
		particles.push_back(particle);

		modelMatrices.push_back(modelMatrix);
		offsets.push_back(offset);
		blendFactors.push_back(blend);
	}

	this->nrOfParticles += nrOfParticles;
}


void Particles::InitBufferData() {

	mat4 modelMatrix = mat4(1.0);
	vec2 offset = vec2(0, 0);
	float blend = 0;

	for (int i = 0; i < nrOfParticles; i++) {

		modelMatrices.push_back(modelMatrix);
		offsets.push_back(offset);
		blendFactors.push_back(blend);
	}

}

void Particles::InitBuffers() {

	glBindVertexArray(quadVAO);

	glGenBuffers(1, &modelMatrixVBO);
	glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * nrOfParticles, &modelMatrices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &offsetVBO);
	glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * nrOfParticles, &offsets[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &blendFactorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, blendFactorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nrOfParticles, &blendFactors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO);
	GLsizei vec4Size = sizeof(vec4);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(vec4Size));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size)); //Why we need all four of these??

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);


	glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
	// Also set instance data
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (GLvoid*)0);
	glVertexAttribDivisor(6, 1); // Tell OpenGL this is an instanced vertex attribute.

	glBindBuffer(GL_ARRAY_BUFFER, blendFactorVBO);
	// Also set instance data
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribDivisor(7, 1); // Tell OpenGL this is an instanced vertex attribute.
	glBindVertexArray(0);
}

void Particles::UpdateBuffers(const vec3& cameraForward, const float& deltaT, shared_ptr<Light> light) {
	
	vec3 u;
	vec3 n;
	vec3 position;

	vec2 offset;
	float blend;
	vec3 averagePosition = vec3(0, 0, 0);

	for (int i = 0; i < nrOfParticles; i++) {

		particles[i].UpdateVelocity(deltaT);
		particles[i].UpdatePosition(deltaT);
		position = particles[i].GetPosition();

		u = cross(-cameraForward, vec3(0, 1, 0));
		u = normalize(u);
		n = cross(u, -cameraForward);

		mat4 modelMatrix = mat4(1.0);
		modelMatrix[0] = vec4(n, 0.0);
		modelMatrix[1] = vec4(u, 0.0);
		modelMatrix[2] = vec4(-cameraForward, 0.0);
		modelMatrix = translate(position) * modelMatrix;

		modelMatrices[i] = modelMatrix;

		offsets[i] = particles[i].GetOffset();
		blendFactors[i] = particles[i].GetBlendFactor();

		averagePosition += position;

	}
	
	averagePosition /= nrOfParticles;
	light->SetPosition(averagePosition);

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * nrOfParticles, &modelMatrices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * nrOfParticles, &offsets[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, blendFactorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * nrOfParticles, &blendFactors[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Particles::ToggleAnimating() {
	if (isAnimating) {
		isAnimating = false;
	}
	else {
		isAnimating = true;
	}
	
}

void Particles::ToggleRendering() {
	
	if (isRendering) {
		isRendering = false;
	}
	else {
		isRendering = true;
	}
}

void Particles::SetAnimating(bool isAnimating) {
	this->isAnimating = isAnimating;
}

void Particles::SetRendering(bool isRendering) {
	this->isRendering = isRendering;
}

bool Particles::IsAnimating() {
	return isAnimating;
}

bool Particles::IsRendering() {
	return isRendering;
}

void Particles::Reset() {
	for (int i = 0; i < nrOfParticles; i++) {

		particles[i].Reset();
	}
}

void Particles::Draw() {
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(false);

	glBindVertexArray(quadVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nrOfParticles); // X triangles of 6 vertices each
	glBindVertexArray(0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);
}

void Particles::AttachShader(const shared_ptr<Shader>& shader) {
	this->shader = shader;
}

shared_ptr<Shader> Particles::GetShader() {
	return shader;
}