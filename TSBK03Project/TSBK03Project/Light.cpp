#include "Light.h"


Light::Light(const shared_ptr<Mesh>& mesh, const vec3& position, const vec3& scale, const vec3& color, const GLfloat& exponent, const std::string& name, const mat4& lightProjection) {
	
	this->mesh = mesh;
	this->position = position;
	this->scale = scale;
	this->color = color;
	this->name = name;
	this->exponent = exponent;
	this->lightProjection = lightProjection;
	target = vec3(0, 0, 0);
	up = vec3(0, 1, 0);
	isLight = false;

}

vec3 Light::GetPosition() {
	return position;
}

vec3 Light::GetColor() {
	return color;
}

GLfloat Light::GetExponent() {
	return exponent;
}

std::string Light::GetName() {
	return name;
}

void Light::SetPosition(const vec3& position) {
	this->position = position;
}

mat4 Light::GetProjectionMatrix() {
	return lightProjection;
}

mat4 Light::GetViewMatrix(const vec3& offset) {
	return lookAt(position + offset, target, up);
}

mat4 Light::GetLightMatrix(const vec3& offset) {
	return lightProjection * lookAt(position + offset, target, up);
}

void Light::UpdateTarget(const vec3& target) {
	this->target = target;
}

void Light::UpdateLightProjection(const mat4& lightProjection) {
	this->lightProjection = lightProjection;
}

shared_ptr<Mesh> Light::GetMesh() {
	return mesh;
}

mat4 Light::GetModelMatrix(const vec3& cameraForward, const bool& mirror) {
	
	vec3 u = cross(-cameraForward, vec3(0, 1, 0));
	u = normalize(u);
	vec3 n = cross(u, -cameraForward);

	mat4 modelMatrix;
	mat4 rotationMatrix = mat4(1.0);
	rotationMatrix[0] = vec4(n, 0.0);
	rotationMatrix[1] = vec4(u, 0.0);
	rotationMatrix[2] = vec4(-cameraForward, 0.0);

	if (mirror) {
		modelMatrix = translate(vec3(position.x, -position.y, position.z)) * rotationMatrix * glm::scale(scale);
	}
	else {
		modelMatrix = translate(position) * rotationMatrix * glm::scale(scale);
	}
	return modelMatrix;
}

void Light::MoveNegativeZ() {
	position += vec3(0.0, 0.0, -1.0);
}

void Light::MovePositiveZ() {
	position += vec3(0.0, 0.0, 1.0);
}

void Light::MoveNegativeX() {
	position += vec3(-1.0, 0.0, 0.0);
}

void Light::MovePositiveX() {
	position += vec3(1.0, 0.0, 0.0);
}

void Light::ToggleRendering() {
	if (isLight) {
		isLight = false;
	}
	else {
		isLight = true;
	}
}

void Light::SetRendering(const bool& isRendering) {
	this->isLight = isRendering;
}

bool Light::IsRendering() {
	return isLight;
}

Light::~Light() {}