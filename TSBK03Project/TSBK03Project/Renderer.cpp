#include "Renderer.h"

Renderer::Renderer(const GLuint& waterheight, shared_ptr<Shader> lightShader, shared_ptr<Shader> depthShader, shared_ptr<FrameBufferObject> reflectionFBO, shared_ptr<FrameBufferObject> refractionFBO, shared_ptr<FrameBufferObject> environmentFBO) {
	this->waterheight = waterheight;
	this->reflectionClipPlane = vec4(0, -1, 0, waterheight+1.2);
	this->reflectionFBO = reflectionFBO;
	this->refractionFBO = refractionFBO;
	this->environmentFBO = environmentFBO;
	this->depthShader = depthShader;
	this->lightShader = lightShader;
	normalClipPlane = vec4(0, 1, 0, 1000);
	testClipPlane = vec4(0, 1, 0, 0);
}

void Renderer::AddLightShader(shared_ptr<Shader> lightShader) {
	this->lightShader = lightShader;
}

void Renderer::AddDepthShader(shared_ptr<Shader> depthShader) {
	this->depthShader = depthShader;
}

void Renderer::AddSkyboxShader(shared_ptr<Shader> skyboxShader) {
	this->skyboxShader = skyboxShader;
}

void Renderer::RenderSkybox(const shared_ptr<Skybox>& skybox, mat4 cameraMatrix, vec4 clipPlane, const bool& value) {

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(false);
	shared_ptr<Shader> shader = skybox->GetShader();
	shader->Bind();
	shader->LoadCameraMatrix(cameraMatrix);
	shader->LoadBoolean(value, "mirror");
	shader->LoadClipPlane(clipPlane);
	skybox->Draw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);
}

void Renderer::RenderParticles(const shared_ptr<Particles>& particles,const mat4& cameraMatrix, const bool& value) {

	shared_ptr<Shader> shader = particles->GetShader();
	shader->Bind();
	shader->LoadCameraMatrix(cameraMatrix);
	shader->LoadBoolean(value, "mirror");
	
	particles->Draw();

}

void Renderer::RenderLight(const shared_ptr<Light>& light, shared_ptr<Camera> camera, vec4 clipPlane, const bool& value) {
	
	lightShader->Bind();
	lightShader->LoadModelMatrix(light->GetModelMatrix(camera->GetForward(), value));
	lightShader->LoadCameraMatrix(camera->GetRotationMatrix());
	lightShader->LoadClipPlane(clipPlane);
	lightShader->LoadBoolean(value, "mirror");
	light->GetMesh()->Draw();
}

void Renderer::RenderEntity(const shared_ptr<Entity>& entity, shared_ptr<Camera> camera, vector<shared_ptr<Light>> lights, vec4 clipPlane, const bool& mirror, const float& dayNightBlend) {

	shared_ptr<Shader> shader = entity->GetShader();
	shader->Bind();
	shader->LoadModelMatrix(entity->GetModelMatrix());
	shader->LoadCameraMatrix(camera->GetCameraMatrix());
	shader->LoadClipPlane(clipPlane);
	shader->LoadBoolean(mirror, "mirror");
	shader->LoadVec3(camera->GetPosition(), "cameraPosition");
	shader->LoadDayNightBlendFactor(dayNightBlend);

	for (auto& lightSource : lights) {
		bool isLightOn = lightSource->IsRendering();
		string lightName = lightSource->GetName();
		shader->SetLightOnOff(isLightOn, lightName);

		if (isLightOn) {
			shader->LoadLight(lightSource->GetPosition(), lightSource->GetColor(), lightSource->GetExponent(), lightName);
		}
		
	}
	mat4 lightMatrix = lights[0]->GetLightMatrix(vec3(0, 0, 0));
	shader->LoadMat4(lightMatrix, "lightMatrix");

	int counter = 1;

	for (auto& fbo : entity->GetFBOs()) {
		shader->LoadFBOTexture(fbo);
	}
	entity->GetMesh()->Draw();
}

void Renderer::RenderEnvironment(const shared_ptr<Skybox>& skybox, const vector<shared_ptr<Entity>>& entities, const vector<shared_ptr<Particles>>& particlesList, shared_ptr<Camera> camera, vector<shared_ptr<Light>> lights) {
	
	//environmentFBO->Bind();
	RenderSkybox(skybox, camera->GetRotationMatrix(), normalClipPlane, false);
	RenderLight(lights[0], camera, normalClipPlane, false);

	for (auto& entity : entities) {
		RenderEntity(entity, camera, lights, normalClipPlane, false, skybox->GetBlendFactor());
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(false);
	for (auto& particles : particlesList) {
		if (particles->IsRendering()) {
			RenderParticles(particles, camera->GetCameraMatrix(), false);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);

	//environmentFBO->Unbind();
}

void Renderer::RenderReflection(const shared_ptr<Skybox>& skybox, const vector<shared_ptr<Entity>>& entities, const vector<shared_ptr<Particles>>& particlesList, shared_ptr<Camera> camera, vector<shared_ptr<Light>> lights) {
	
	reflectionFBO->Bind();
	RenderSkybox(skybox, camera->GetRotationMatrix(), reflectionClipPlane, true);
	RenderLight(lights[0], camera, reflectionClipPlane, true);

	for (auto& entity : entities) {
		RenderEntity(entity, camera, lights, reflectionClipPlane, true, skybox->GetBlendFactor());
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(false);
	for (auto& particles : particlesList) {
		if (particles->IsRendering()) {
			RenderParticles(particles, camera->GetCameraMatrix(), true);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);
	reflectionFBO->Unbind();
}

void Renderer::RenderRefraction(const shared_ptr<Skybox>& skybox, const vector<shared_ptr<Entity>>& entities, shared_ptr<Camera> camera, vector<shared_ptr<Light>> lights) {

	refractionFBO->Bind();

	for (auto& entity : entities) {
		RenderEntity(entity, camera, lights, reflectionClipPlane, false, skybox->GetBlendFactor());
	}
	refractionFBO->Unbind();
}

void Renderer::RenderDepthMaps(const vector<shared_ptr<Entity>>& entities, vector<shared_ptr<Light>> lights) {
	
	int offset = 0;
	for (auto& lightDepthFBO : lightDepthFBOS) {

		lightDepthFBO->Bind();
		depthShader->Bind();
		depthShader->LoadCameraMatrix(lights[0]->GetViewMatrix(vec3(0, 0, 2.0 * offset)));
		depthShader->LoadProjectionMatrix(lights[0]->GetProjectionMatrix());

		for (auto& entity : entities) {

			depthShader->LoadModelMatrix(entity->GetModelMatrix());
			entity->GetMesh()->Draw();
		}
		offset++;
		lightDepthFBO->Unbind();
	}
}

void Renderer::RenderBottomDepth(const shared_ptr<Entity>& entity,const mat4& cameraMatrix, const mat4& projectionMatrix) {

		waterDepthFBO->Bind();
		depthShader->Bind();
		depthShader->LoadCameraMatrix(cameraMatrix);
		depthShader->LoadProjectionMatrix(projectionMatrix);
		depthShader->LoadModelMatrix(entity->GetModelMatrix());
		entity->GetMesh()->Draw();

		waterDepthFBO->Unbind();
}

void Renderer::AddLightDepthFBO(const shared_ptr<FrameBufferObject>& lightDepthFBO) {
	lightDepthFBOS.push_back(lightDepthFBO);
}

void Renderer::AddWaterDepthFBO(const shared_ptr<FrameBufferObject>& waterDepthFBO) {
	this->waterDepthFBO = waterDepthFBO;
}

Renderer::~Renderer()
{
}
