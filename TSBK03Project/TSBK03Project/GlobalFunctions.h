#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include "Display.h"
#include <math.h>
#include "Camera.h"
#include "Entity.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Terrain.h"
#include "Light.h"
#include "Particles.h"

struct EventHolder {

public:
	EventHolder() {
		renderEnvironment = false;
		renderFire = false;
		renderParticles = false;
		renderParticleFBOS = false;
		renderFireFBO = false;
		renderNormalmap = false;
		renderReflection = false;
		renderRefraction = false;
		renderBottomDepth = false;
		renderShadows = false;
		renderReflectionFBO = false;
		renderRefractionFBO = false;
		renderBottomDepthFBO = false;
		renderShadowFBO = false;
		waterMoving = false;
	}

	inline void SetRenderEnvironment(bool value) {
		renderEnvironment = value;
	}

	inline void ToggleRenderEnvironment() {
		if (renderEnvironment) {
			renderEnvironment = false;
			renderNormalmap = false;
			renderReflection = false;
			renderRefraction = false;
			renderBottomDepth = false;
			renderShadows = false;
			renderReflectionFBO = false;
			renderRefractionFBO = false;
			renderBottomDepthFBO = false;
			renderShadowFBO = false;
			waterMoving = false;
		} else {
			renderEnvironment = true;
		}	
	}

	inline void ToggleRenderReflectionFBO() {
		if (renderReflectionFBO) {
			renderReflectionFBO = false;
		}
		else {
			renderReflectionFBO = true;
		}
	}

	inline void ToggleRenderRefractionFBO() {
		if (renderRefractionFBO) {
			renderRefractionFBO = false;
		}
		else {
			renderRefractionFBO = true;
		}
	}

	inline void ToggleRenderBottomDepthFBO() {
		if (renderBottomDepthFBO) {
			renderBottomDepthFBO = false;
		}
		else {
			renderBottomDepthFBO = true;
		}
	}

	inline void ToggleRenderShadowFBO() {
		if (renderShadowFBO) {
			renderShadowFBO = false;
		}
		else {
			renderShadowFBO = true;
		}
	}

	inline void SetRenderFire(bool value) {
		renderFire = value;
	}

	inline void ToggleRenderFire() {
		if (renderFire) {
			renderFire = false;
		}
		else {
			renderFire = true;
		}
	}

	inline void ToggleRenderParticles() {
		if (renderParticles) {
			renderParticles = false;
		}
		else {
			renderParticles = true;
		}
	}

	inline void SetRenderParticleFBOS(bool value) {
		renderParticleFBOS = value;
	}

	inline void ToggleRenderParticleFBOS() {
		if (renderParticleFBOS) {
			renderParticleFBOS = false;
		}
		else {
			renderParticleFBOS = true;
		}
	}

	inline void ToggleRenderFireFBO() {
		if (renderFireFBO) {
			renderFireFBO = false;
		}
		else {
			renderFireFBO = true;
		}
	}

	inline void ToggleNormalmap() {
		if (renderNormalmap) {
			renderNormalmap = false;
		}
		else {
			renderNormalmap = true;
		}
	}

	inline void ToggleReflection() {
		if (renderReflection) {
			renderReflection = false;
		}
		else {
			renderReflection = true;
		}
	}

	inline void ToggleRefraction() {
		if (renderRefraction) {
			renderRefraction = false;
		}
		else {
			renderRefraction = true;
		}
	}

	inline void ToggleBottomDepth() {
		if (renderBottomDepth) {
			renderBottomDepth = false;
		}
		else {
			renderBottomDepth = true;
		}
	}

	inline void ToggleShadows() {
		if (renderShadows) {
			renderShadows = false;
		}
		else {
			renderShadows = true;
		}
	}

	inline void ToggleWaterMotion() {
		if (waterMoving) {
			waterMoving = false;
		} else {
			waterMoving = true;
		}
	}

	inline bool WaterMoving() {
		return waterMoving;
	}

	inline void SetReflection(bool value) {
		
		renderReflection = value;
	}

	inline void SetRefraction(bool value) {
		renderRefraction = value;
	}

	inline void SetBottomDepth(bool value) {
		renderBottomDepth = value;
	}

	inline void SetShadows(bool value) {
		renderShadows = value;
	}

	inline bool RenderEnvironment() {
		return renderEnvironment;
	}

	inline bool RenderParticles() {
		return renderParticles;
	}

	inline bool RenderParticleFBOS() {
		return renderParticleFBOS;
	}

	inline bool RenderFire() {
		return renderFire;
	}

	inline bool RenderFireFBO() {
		return renderFireFBO;
	}

	inline bool RenderNormalmap() {
		return renderNormalmap;
	}

	inline bool RenderReflection() {
		return renderReflection;
	}

	inline bool RenderRefraction() {
		return renderRefraction;
	}

	inline bool RenderBottomDepth() {
		return renderBottomDepth;
	}

	inline bool RenderShadows() {
		return renderShadows;
	}

	inline bool RenderReflectionFBO() {
		return renderReflectionFBO;
	}

	inline bool RenderRefractionFBO() {
		return renderRefractionFBO;
	}

	inline bool RenderBottomDepthFBO() {
		return renderBottomDepthFBO;
	}

	inline bool RenderShadowFBO() {
		return renderShadowFBO;
	}

private:
	bool renderEnvironment;
	bool renderReflectionFBO;
	bool renderRefractionFBO;
	bool renderBottomDepthFBO;
	bool renderShadowFBO;
	bool renderParticles;
	bool renderParticleFBOS;
	bool renderFire;
	bool renderFireFBO;
	bool renderNormalmap;
	bool renderReflection;
	bool renderRefraction;
	bool renderBottomDepth;
	bool renderShadows;
	bool waterMoving;
};

bool HandleEvents(const shared_ptr<Display> display, const shared_ptr<Skybox> skybox, const shared_ptr<Camera> camera, const shared_ptr<Light>& sunLight,
	const shared_ptr<Light>& fireLight, const shared_ptr<Light>& starsLight, const shared_ptr<Particles>& fireParticles, 
	const shared_ptr<Particles>& starsParticles, shared_ptr<EventHolder>& eventHolder, const shared_ptr<Entity>& waterEntity, const vector<shared_ptr<Shader>> waterShaders);

void UpdateLightMatrices(const shared_ptr<Camera>& camera, const shared_ptr<Light>& light, const float& shadowDistance);

void Diamond(vec3 *vertexArray, const GLuint& x, const GLuint& z, const GLuint offset, GLuint size, float scale, float borderHeight);
void Square(vec3 *vertexArray, GLuint x, GLuint z, GLuint offset, GLuint size, float scale, float borderHeight);

Terrain GenerateTerrain(const GLuint& size);