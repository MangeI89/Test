#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <vector>
#include "Entity.h"
#include "Light.h"
#include "Particles.h"

using namespace std;
using namespace glm;

class Renderer
{
public:
	Renderer(const GLuint& waterheight, shared_ptr<Shader> lightShader, shared_ptr<Shader> depthShader, shared_ptr<FrameBufferObject> reflectionFBO, shared_ptr<FrameBufferObject> refractionFBO, shared_ptr<FrameBufferObject> environmentFBO);

	void AddLightShader(shared_ptr<Shader> lightShader);
	void AddDepthShader(shared_ptr<Shader> depthShader);
	void AddSkyboxShader(shared_ptr<Shader> skyboxShader);

	void RenderSkybox(const shared_ptr<Skybox>& skybox, mat4 cameraMatrix, vec4 clipPlane, const bool& value);
	void RenderLight(const shared_ptr<Light>& light, shared_ptr<Camera> camera, vec4 clipPlane, const bool& value);
	void RenderEntity(const shared_ptr<Entity>& entity, shared_ptr<Camera> camera, vector<shared_ptr<Light>> lights, vec4 clipPlane, const bool& mirror, const float& dayNightBlend);
	void RenderEnvironment(const shared_ptr<Skybox>& skybox, const vector<shared_ptr<Entity>>& entities, const vector<shared_ptr<Particles>>& particlesList, shared_ptr<Camera> camera, vector<shared_ptr<Light>> lights);
	void RenderParticles(const shared_ptr<Particles>& particles, const mat4& cameraMatrix, const bool& value);
	void RenderReflection(const shared_ptr<Skybox>& skybox, const vector<shared_ptr<Entity>>& entities, const vector<shared_ptr<Particles>>& particles, shared_ptr<Camera> camera, vector<shared_ptr<Light>> lights);
	void RenderRefraction(const shared_ptr<Skybox>& skybox, const vector<shared_ptr<Entity>>& entities, shared_ptr<Camera> camera, vector<shared_ptr<Light>> lights);
	void RenderDepthMaps(const vector<shared_ptr<Entity>>& entities, vector<shared_ptr<Light>> lights);
	void RenderBottomDepth(const shared_ptr<Entity>& entity, const mat4& cameraMatrix, const mat4& projectionMatrix);
	void AddLightDepthFBO(const shared_ptr<FrameBufferObject>& lightDepthFBO);
	void AddWaterDepthFBO(const shared_ptr<FrameBufferObject>& waterDepthFBO);
	~Renderer();

private:
	GLuint waterheight;
	vec4 reflectionClipPlane;
	vec4 normalClipPlane;
	vec4 testClipPlane;
	shared_ptr<Shader> depthShader;
	shared_ptr<Shader> lightShader;
	shared_ptr<Shader> skyboxShader;
	shared_ptr<FrameBufferObject> reflectionFBO;
	shared_ptr<FrameBufferObject> refractionFBO;
	shared_ptr<FrameBufferObject> environmentFBO;
	shared_ptr<FrameBufferObject> waterDepthFBO;
	vector<shared_ptr<FrameBufferObject>> lightDepthFBOS;
};

