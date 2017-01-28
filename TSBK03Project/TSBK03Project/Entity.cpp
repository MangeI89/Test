#include "Entity.h"

Entity::Entity() {}

Entity::Entity(const Mesh& mesh) {
	//m_mesh = mesh;
}

Entity::Entity(const shared_ptr<Mesh>& mesh, const vec3& position, const vec3& velocity, const vec3& rotation, const vec3& scale) {
	this->mesh = mesh;
	this->currentPosition = position;
	this->initialPosition = position;
	this->currentVelocity = velocity;
	this->initialVelocity = velocity;
	this->rotation = rotation;
	this->scale = scale;

	lifeTime = 0;
	resetWhenZero = false;
}

Entity::~Entity() {}

void Entity::AttachShader(const shared_ptr<Shader>& shader) {
	this->shader = shader;
}

void Entity::AddTexture(const shared_ptr<Texture>& texture) {
	textures.push_back(texture);
}

void Entity::AddFBO(const shared_ptr<FrameBufferObject>& fbo) {
	fbos.push_back(fbo);
}

void Entity::AddSkybox(const shared_ptr<Skybox>& skybox) {
	skyboxes.push_back(skybox);
	currentSkybox = skybox;
}

shared_ptr<Shader> Entity::GetShader() {
	return shader;
}

vector<shared_ptr<Texture>> Entity::GetTextures() {
	return textures;
}

vector<shared_ptr<FrameBufferObject>> Entity::GetFBOs() {
	return fbos;
}

vector<shared_ptr<Skybox>> Entity::GetSkyboxes() {
	return skyboxes;
}

shared_ptr<Skybox> Entity::GetCurrentSkybox() {
	return currentSkybox;
}

void Entity::ChooseSkybox(const GLuint& unit) {
	currentSkybox = skyboxes[unit];
}

vec3 Entity::GetPosition() const { return currentPosition; }
vec3 Entity::GetRotation() { return rotation; }
vec3 Entity::GetScale() { return scale; }

mat4 Entity::GetModelMatrix() {

	mat4 translationMatrix = translate(currentPosition);

	mat4 rotX = rotate(radians(rotation.x), vec3(1, 0, 0));
	mat4 rotY = rotate(radians(rotation.y), vec3(0, 1, 0));
	mat4 rotZ = rotate(radians(rotation.z), vec3(0, 0, 1));

	mat4 rotationMatrix = rotZ * rotY * rotX;

	mat4 scaleMatrix = glm::scale(scale);

	return translationMatrix * rotationMatrix * scaleMatrix;
}

mat4 Entity::GetBillboardModelMatrix(const vec3& cameraForward) {

	vec3 u = cross(-cameraForward, vec3(0, 1, 0));
	u = normalize(u);
	vec3 n = cross(u, -cameraForward);

	mat4 modelMatrix = mat4(1.0);
	modelMatrix[0] = vec4(n, 0.0);
	modelMatrix[1] = vec4(u, 0.0);
	modelMatrix[2] = vec4(-cameraForward, 0.0);
	return translate(currentPosition) * modelMatrix;
}

shared_ptr<Mesh> Entity::GetMesh() {
	return mesh;
}

void Entity::UpdatePosition(const float& deltaT) {
	currentPosition = currentPosition + currentVelocity * deltaT;
	if (currentPosition.y < initialPosition.y) {
		currentPosition = initialPosition;
		currentVelocity = initialVelocity;
		lifeTime = 0;
	}

	lifeTime += deltaT;
}

void Entity::UpdateVelocity(const float& deltaT) {
	currentVelocity.y = currentVelocity.y - 9.82 * deltaT;
}

float Entity::GetBlendFactor() {
	double fraction, intFactor;
	double mod = lifeTime / 0.125;

	fraction = modf(mod, &intFactor);
	return fraction;
}

vec2 Entity::GetOffset() {
	
	double xFraction1, yFraction1, xFraction2, yFraction2, xInt1, yInt1, xInt2, yInt2;
	double xMod = lifeTime / 0.125;
	double yMod = lifeTime / 0.5;

	if (yMod > 4.0) { resetWhenZero = true; }
	xFraction1 = modf(xMod, &xInt1);
	yFraction1 = modf(yMod, &yInt1);
	
	xFraction2 = modf(xInt1 / 4.0, &xInt2);
	yFraction2 = modf(yInt1 / 4.0, &yInt2);

	return vec2(xInt1, yInt1);
}

void Entity::SetPosition(const vec3& position) { this->currentPosition = position; }
void Entity::SetRotation(const vec3& rotation) { this->rotation = rotation; }
void Entity::SetScale(const vec3& scale) { this->scale = scale; }


