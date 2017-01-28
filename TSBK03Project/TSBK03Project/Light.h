#pragma once
#include <glm\glm.hpp>
#include <iostream>
#include <GL\glew.h>
#include <glm/gtx/transform.hpp>
#include <memory>
#include "Mesh.h"

using namespace std;
using namespace glm;

class Light
{
public:
	Light(const shared_ptr<Mesh>& mesh, const vec3& position, const vec3& scale, const vec3& color,const GLfloat& exponent, const string& name, const mat4& lightProjection);

	vec3 GetPosition();
	vec3 GetColor();
	GLfloat GetExponent();
	string GetName();
	void SetPosition(const vec3& position);
	mat4 GetProjectionMatrix();
	mat4 GetViewMatrix(const vec3& offset);
	mat4 GetLightMatrix(const vec3& offset);

	void UpdateTarget(const vec3& target);
	void UpdateLightProjection(const mat4& lightProjection);
	shared_ptr<Mesh> GetMesh();
	mat4 GetModelMatrix(const vec3& cameraForward, const bool& mirror);
	void MoveNegativeZ();
	void MovePositiveZ();
	void MoveNegativeX();
	void MovePositiveX();
	void ToggleRendering();
	void SetRendering(const bool& isRendering);
	bool IsRendering();

	~Light();

private:
	shared_ptr<Mesh> mesh;
	vec3 position;
	vec3 scale;
	vec3 color;
	GLfloat exponent;
	string name;
	vec3 target;
	vec3 up;
	mat4 lightProjection;
	bool isLight;
};

