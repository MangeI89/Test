#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "Shader.h"
#include "Skybox.h"
#include "Texture.h"
#include "FrameBufferObject.h"
#include "Mesh.h"

using namespace std;
using namespace glm;

class Entity
{
public:
	Entity();
	Entity(const Mesh& mesh);
	Entity(const shared_ptr<Mesh>& mesh, const vec3& position, const vec3& velocity, const vec3& rotation, const vec3& scale);
	
	void AttachShader(const shared_ptr<Shader>& shader);
	void AddTexture(const shared_ptr<Texture>& texture);
	void AddFBO(const shared_ptr<FrameBufferObject>& fbo);
	void AddSkybox(const shared_ptr<Skybox>& skybox);

	void UpdatePosition(const float& deltaT);
	void UpdateVelocity(const float& deltaT);
	float GetBlendFactor();
	vec2 GetOffset();
	void SetPosition(const vec3& position);
	void SetRotation(const vec3& rotation);
	void SetScale(const vec3& scale);

	shared_ptr<Shader> GetShader();
	vector<shared_ptr<Texture>> GetTextures();
	vector<shared_ptr<Skybox>> GetSkyboxes();
	vector<shared_ptr<FrameBufferObject>> Entity::GetFBOs();
	shared_ptr<Skybox> GetCurrentSkybox();
	void Entity::ChooseSkybox(const GLuint& unit);

	vec3 GetPosition() const;
	vec3 GetRotation();
	vec3 GetScale();
	mat4 GetModelMatrix();
	mat4 GetBillboardModelMatrix(const vec3& cameraForward);
	shared_ptr<Mesh> GetMesh();

	~Entity();

private:
	shared_ptr<Shader> shader;
	shared_ptr<Mesh> mesh;
	vector<shared_ptr<Texture>> textures;
	vector<shared_ptr<FrameBufferObject>> fbos;
	vector<shared_ptr<Skybox>> skyboxes;
	shared_ptr<Skybox> currentSkybox;
	vec3 currentPosition;
	vec3 initialPosition;
	vec3 currentVelocity;
	vec3 initialVelocity;
	vec3 rotation;
	vec3 scale;

	float lifeTime;
	bool resetWhenZero;
};

