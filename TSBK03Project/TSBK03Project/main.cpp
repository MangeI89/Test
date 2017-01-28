#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <SDL2\SDL.h>
#include <memory>
#include <vector>
#include <assimp\cimport.h>
#include <assimp\postprocess.h>
#include "GlobalFunctions.h"
#include "Display.h"
#include "Mesh.h"
#include "Entity.h"
#include "Shader.h"
#include "Texture.h"
#include "Skybox.h"
#include "Projection.h"
#include "Camera.h"
#include "Light.h"
#include "FrameBufferObject.h"
#include "Renderer.h"
#include "Particles.h"

using namespace std;
using namespace glm;

#define WIDTH 1500
#define HEIGHT 800
#define NEAR 0.1f
#define FAR 500.0f
#define FOV 70.0f

int main(int argc, char** argv) {

	Projection projection(FOV, (float)WIDTH / (float)HEIGHT, NEAR, FAR);
	mat4 lightProjection = ortho(-60.0f, 60.0f, -25.0f, 25.0f, 1.0f, 300.0f); //Fix variables for this
	GLuint terrainGenerationLevel = 7;
	float terrainScale = 1.0;
	Terrain terrain = GenerateTerrain(terrainGenerationLevel);
	float terrainSize = ((float)(pow(2, terrainGenerationLevel)) + 1.0) * terrainScale;
	const GLuint nrOfSimpleFireParticles = 500;
		
	//THIS VALUE DETERMINES THE NR OF PARTICLES RENDERED WHEN UPDATING PARAMETERS WITH FBOS-----------------------------------------------
	
	const GLuint nrOfMassParticles = 800000;

	//------------------------------------------------------------------------------------------------------------------------------------

	vec3 starsParticlePositions = vec3(150, 0, 0);
	vec3 fireParticlePositions = vec3(150, 0, 0);

	vec3 sunPosition = vec3(10, 50, 100);
	vec3 firePosition = vec3(10, 2, -40), fireVelocity = vec3(0,0,0), fireRandomness = vec3(7, 2, 7);
	vec3 starsPosition = vec3(0, 150, 100), starsVelocity = vec3(0, 0, 0), starsRandomness = vec3(7, 2, 7);;
	vec3 fireworkPosition = vec3(0, 0, 100), fireworkVelocity = vec3(0, 80, 0), fireworkRandomness = vec3(0, 0, 0);;
	float shadowDistance = 300.0;
	
	shared_ptr<Display> display(new Display(WIDTH, HEIGHT, "Project"));
	shared_ptr<Camera> camera(new Camera(vec3(-50, 200, 0)));
	//shared_ptr<Camera> camera(new Camera(vec3(-10, 5, -10)));

	shared_ptr<EventHolder> eventHolder(new EventHolder());

	string skyboxTextures[6] = { "./res/skyboxes/stonegods/ft.tga",
		"./res/skyboxes/stonegods/bk.tga",
		"./res/skyboxes/stonegods/up.tga",
		"./res/skyboxes/stonegods/dn.tga",
		"./res/skyboxes/stonegods/rt.tga",
		"./res/skyboxes/stonegods/lf.tga" };

	string skyboxTextures2[6] = { "./res/skyboxes/mp_blood/ft.tga",
		"./res/skyboxes/mp_blood/bk.tga",
		"./res/skyboxes/mp_blood/up.tga",
		"./res/skyboxes/mp_blood/dn.tga",
		"./res/skyboxes/mp_blood/rt.tga",
		"./res/skyboxes/mp_blood/lf.tga" };

	Vertex waterVertices[] = { Vertex(vec3(-0.5,0,-0.5), vec2(0,1), vec3(0,1,0)),
		Vertex(vec3(-0.5,0,0.5), vec2(0,0), vec3(0,1,0)),
		Vertex(vec3(0.5,0,-0.5), vec2(1,1), vec3(0,1,0)),
		Vertex(vec3(0.5,0,0.5), vec2(1,0), vec3(0,1,0)) };

	Vertex sunVertices[] = { Vertex(vec3(-1.0, 1.0, 0), vec2(0,1), vec3(0,0,1)),
		Vertex(vec3(1.0, -1.0,0), vec2(1,0), vec3(0,0,1)),
		Vertex(vec3(-1.0, -1.0,0), vec2(0,0), vec3(0,0,1)),
		Vertex(vec3(1.0, 1.0,0), vec2(1,1), vec3(0,0,1))};

	unsigned int waterIndices[] = { 0, 1, 2, 1, 3, 2 };
	unsigned int sunIndices[] = { 0, 1, 2, 0, 1, 3 };
	unsigned int numIndices = 6;
	unsigned int numVertices = 4;

	bool instancing = true;

	//FOR PRESENTATION
	shared_ptr<Shader> water0Shader(new Shader("./res/shaders/water0Shader"));
	shared_ptr<Shader> water1Shader(new Shader("./res/shaders/water1Shader"));
	shared_ptr<Shader> water2Shader(new Shader("./res/shaders/water2Shader"));
	shared_ptr<Shader> water3Shader(new Shader("./res/shaders/water3Shader"));
	shared_ptr<Shader> water4Shader(new Shader("./res/shaders/water4Shader"));
	shared_ptr<Shader> water5Shader(new Shader("./res/shaders/water5Shader"));
	shared_ptr<Shader> water6Shader(new Shader("./res/shaders/water6Shader"));
	shared_ptr<Shader> waterShader(new Shader("./res/shaders/waterShader"));
	vector<shared_ptr<Shader>> waterShaders;
	waterShaders.push_back(water0Shader);
	waterShaders.push_back(water1Shader);
	waterShaders.push_back(water2Shader);
	waterShaders.push_back(water3Shader);
	waterShaders.push_back(water4Shader);
	waterShaders.push_back(water5Shader);
	waterShaders.push_back(water6Shader);
	waterShaders.push_back(waterShader);


	shared_ptr<Shader> skyboxShader(new Shader("./res/shaders/skyboxShader"));
	shared_ptr<Shader> monkeyShader(new Shader("./res/shaders/monkeyShader"));
	shared_ptr<Shader> depthShader(new Shader("./res/shaders/depthShader"));
	shared_ptr<Shader> fboShader(new Shader("./res/shaders/fboShader"));
	shared_ptr<Shader> showLightDepthShader(new Shader("./res/shaders/showLightDepthShader"));
	shared_ptr<Shader> showWaterDepthShader(new Shader("./res/shaders/showWaterDepthShader"));
	shared_ptr<Shader> showReflectionShader(new Shader("./res/shaders/showReflectionShader"));
	shared_ptr<Shader> showRefractionShader(new Shader("./res/shaders/showRefractionShader"));
	shared_ptr<Shader> showVelocityShader(new Shader("./res/shaders/showVelocityShader"));
	shared_ptr<Shader> showPositionShader(new Shader("./res/shaders/showPositionShader"));
	shared_ptr<Shader> showOffsetShader(new Shader("./res/shaders/showOffsetShader"));
	shared_ptr<Shader> finalShader(new Shader("./res/shaders/finalShader"));
	shared_ptr<Shader> lightShader(new Shader("./res/shaders/lightShader"));
	shared_ptr<Shader> terrainShader(new Shader("./res/shaders/terrainShader"));
	shared_ptr<Shader> fireMassParticleShader(new Shader("./res/shaders/fireMassParticleShader"));
	shared_ptr<Shader> starsMassParticleShader(new Shader("./res/shaders/starsMassParticleShader"));
	shared_ptr<Shader> fireSimpleShader(new Shader("./res/shaders/fireSimpleShader"));
	shared_ptr<Shader> starsSimpleShader(new Shader("./res/shaders/starsSimpleShader"));
	shared_ptr<Shader> velocityShader(new Shader("./res/shaders/velocityShader"));
	shared_ptr<Shader> positionShader(new Shader("./res/shaders/positionShader"));
	shared_ptr<Shader> initVelocityShader(new Shader("./res/shaders/initVelocityShader"));
	shared_ptr<Shader> initPositionShader(new Shader("./res/shaders/initPositionShader"));
	shared_ptr<Shader> particleOffsetInfoShader(new Shader("./res/shaders/particleOffsetInfoShader"));
	shared_ptr<Shader> particleTexCoordInfoShader(new Shader("./res/shaders/particleTexCoordInfoShader"));

	shared_ptr<Mesh> terrainMesh(new Mesh(terrain.GetVertexArray(), terrain.GetNumVertices(), terrain.GetIndexArray(), terrain.GetNumIndices()));
	shared_ptr<Mesh> waterMesh(new Mesh(waterVertices, numVertices, waterIndices, numIndices));
	shared_ptr<Mesh> sunMesh(new Mesh(sunVertices, numVertices, sunIndices, numIndices));
	shared_ptr<Mesh> skyboxMesh(new Mesh("./res/objects/skybox4.obj"));
	shared_ptr<Mesh> monkeyMesh(new Mesh("./res/objects/monkey3.obj"));

	shared_ptr<Light> sunLight(new Light(sunMesh, sunPosition, vec3(25,25,1), vec3(1.0, 1.0, 1.0), 30, "sun", lightProjection));
	shared_ptr<Light> fireLight(new Light(sunMesh, firePosition, vec3(25, 25, 1), vec3(0.88, 0.35, 0.13), 30, "fire", lightProjection)); //Shouldn't load mesh if not used
	shared_ptr<Light> starsLight(new Light(sunMesh, starsPosition, vec3(1, 1, 1), vec3(0.54, 0.17, 0.89), 30, "stars", lightProjection));
	vector<shared_ptr<Light>> lights;
	lights.push_back(sunLight);
	lights.push_back(fireLight);
	lights.push_back(starsLight);

	shared_ptr<FrameBufferObject> reflectionFBO(new FrameBufferObject(16, "reflectionTex", WIDTH, HEIGHT));
	shared_ptr<FrameBufferObject> refractionFBO(new FrameBufferObject(17, "refractionTex", WIDTH, HEIGHT));
	shared_ptr<FrameBufferObject> lightDepthFBO(new FrameBufferObject(18, "lightDepthTex", WIDTH, HEIGHT, "depth"));
	shared_ptr<FrameBufferObject> lightDepthFBO2(new FrameBufferObject(19, "lightDepthTex2", WIDTH, HEIGHT, "depth"));
	shared_ptr<FrameBufferObject> lightDepthFBO3(new FrameBufferObject(20, "lightDepthTex3", WIDTH, HEIGHT, "depth"));
	shared_ptr<FrameBufferObject> lightDepthFBO4(new FrameBufferObject(21, "lightDepthTex4", WIDTH, HEIGHT, "depth"));
	shared_ptr<FrameBufferObject> lightDepthFBO5(new FrameBufferObject(22, "lightDepthTex5", WIDTH, HEIGHT, "depth"));
	shared_ptr<FrameBufferObject> waterDepthFBO(new FrameBufferObject(23, "waterDepthTex", WIDTH, HEIGHT, "depth"));
	shared_ptr<FrameBufferObject> environmentFBO(new FrameBufferObject(24, "environmentTex", WIDTH, HEIGHT));
	shared_ptr<FrameBufferObject> velocityFBO(new FrameBufferObject(25, "velocityTex", WIDTH, HEIGHT));
	shared_ptr<FrameBufferObject> velocityFBO2(new FrameBufferObject(26, "velocityTex", WIDTH, HEIGHT));
	shared_ptr<FrameBufferObject> positionFBO(new FrameBufferObject(27, "positionTex", WIDTH, HEIGHT));
	shared_ptr<FrameBufferObject> positionFBO2(new FrameBufferObject(28, "positionTex", WIDTH, HEIGHT));
	shared_ptr<FrameBufferObject> particleOffsetInfoFBO(new FrameBufferObject(29, "particleOffsetInfoTex", WIDTH, HEIGHT));
	shared_ptr<FrameBufferObject> particleTexCoordInfoFBO(new FrameBufferObject(30, "particleTexCoordInfoTex", WIDTH, HEIGHT));

	shared_ptr<Texture> blendTexture(new Texture("./res/textures/blend.png", "blendTex", 0));
	shared_ptr<Texture> crackedTexture(new Texture("./res/textures/cracked.jpg", "crackedTex", 1));
	shared_ptr<Texture> rockTexture(new Texture("./res/textures/rock.jpg", "rockTex", 3));
	shared_ptr<Texture> grassTexture(new Texture("./res/textures/grass.jpg", "grassTex",4));
	shared_ptr<Texture> normalTexture(new Texture("./res/textures/normal.png", "normalTex", 6));
	shared_ptr<Texture> dudvTexture(new Texture("./res/textures/dudv.png", "dudvTex", 7));
	shared_ptr<Texture> starTexture(new Texture("./res/textures/star.png", "starTex", 9));
	shared_ptr<Texture> fireTexture(new Texture("./res/textures/fire.png", "fireTex", 10));
	shared_ptr<Texture> terrainRockTexture(new Texture("./res/textures/terrainRock.jpg", "terrainRockTex", 11));
	shared_ptr<Texture> stoneTexture(new Texture("./res/textures/terrainRock.jpg", "stoneTex", 13));

	shared_ptr<Skybox> skybox(new Skybox(skyboxMesh, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1)));

	shared_ptr<Entity> monkeyEntity(new Entity(monkeyMesh, vec3(20, 10, 0), vec3(0, 0, 0), vec3(0, 0, 45), vec3(2, 2, 2)));
	shared_ptr<Entity> waterEntity(new Entity(waterMesh, vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0), vec3(terrainSize, 1, terrainSize)));
	shared_ptr<Entity> teddyEntity(new Entity(monkeyMesh, vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 0, 90), vec3(2, 2, 2)));

	vector<vec2> vertices;

	vertices.push_back(vec2(-0.5f, 0.5f));
	vertices.push_back(vec2(0.5f, -0.5f));
	vertices.push_back(vec2(-0.5f, -0.5f));

	vertices.push_back(vec2(-0.5f, 0.5f));
	vertices.push_back(vec2(0.5f, -0.5f));
	vertices.push_back(vec2(0.5f, 0.5f));

	vector<vec2> texCoords;
	texCoords.push_back(vec2(0.0f, 1.0f));
	texCoords.push_back(vec2(1.0f, 0.0f));
	texCoords.push_back(vec2(0.0f, 0.0f));

	texCoords.push_back(vec2(0.0f, 1.0f));
	texCoords.push_back(vec2(1.0f, 0.0f));
	texCoords.push_back(vec2(1.0f, 1.0f));

	vector<shared_ptr<Particles>> particlesList;

	shared_ptr<Particles> simpleFire(new Particles(vertices, texCoords));
	shared_ptr<Particles> massParticles(new Particles(vertices, texCoords));

	particlesList.push_back(simpleFire);

	simpleFire->GenerateParticles(firePosition, fireVelocity, fireRandomness, nrOfSimpleFireParticles);
	massParticles->GenerateParticles(starsPosition, starsVelocity, starsRandomness, nrOfMassParticles);

	simpleFire->InitBuffers();

	Renderer renderer(0, lightShader, depthShader, reflectionFBO, refractionFBO, environmentFBO);
	renderer.AddLightDepthFBO(lightDepthFBO);
	renderer.AddLightDepthFBO(lightDepthFBO2);
	renderer.AddLightDepthFBO(lightDepthFBO3);
	renderer.AddLightDepthFBO(lightDepthFBO4);
	renderer.AddLightDepthFBO(lightDepthFBO5);
	renderer.AddWaterDepthFBO(waterDepthFBO);

	vector<shared_ptr<Entity>> reflectionEntities;
	vector<shared_ptr<Entity>> refractionEntities;
	vector<shared_ptr<Entity>> environmentEntities;

	shared_ptr<Entity> fboEntity(new Entity(waterMesh, vec3(0.6, 0.3, -1), vec3(0, 0, 0), vec3(90, 0, 0), vec3(0.5, 1, 0.3)));
	shared_ptr<Entity> finalEntity(new Entity(waterMesh, vec3(0, 0, -1), vec3(0, 0, 0), vec3(90, 0, 0), vec3(1.78, 1, 0.95)));

	shared_ptr<Entity> terrainEntity(new Entity(terrainMesh, vec3(-(terrainSize / 2.0), 0, -(terrainSize / 2.0)), vec3(0, 0, 0), vec3(0, 0, 0), vec3(terrainScale, 1, terrainScale)));

	reflectionEntities.push_back(monkeyEntity);
	reflectionEntities.push_back(teddyEntity);
	reflectionEntities.push_back(terrainEntity);

	refractionEntities.push_back(terrainEntity);

	environmentEntities.push_back(monkeyEntity);
	environmentEntities.push_back(teddyEntity);
	environmentEntities.push_back(terrainEntity);
	environmentEntities.push_back(waterEntity);

	skybox->AttachShader(skyboxShader);
	skybox->AddSkyboxTextureSet(skyboxTextures, "day");
	skybox->AddSkyboxTextureSet(skyboxTextures2, "night");

	monkeyEntity->AttachShader(monkeyShader);

	waterEntity->AttachShader(water0Shader);
	waterEntity->AddFBO(reflectionFBO);
	waterEntity->AddFBO(refractionFBO);
	waterEntity->AddFBO(lightDepthFBO);
	waterEntity->AddFBO(lightDepthFBO2);
	waterEntity->AddFBO(lightDepthFBO3);
	waterEntity->AddFBO(lightDepthFBO4);
	waterEntity->AddFBO(lightDepthFBO5);
	waterEntity->AddFBO(waterDepthFBO);
	
	fboEntity->AttachShader(fboShader);
	fboEntity->AddFBO(lightDepthFBO);

	teddyEntity->AttachShader(monkeyShader);
	terrainEntity->AttachShader(terrainShader);

	finalEntity->AttachShader(finalShader);

	simpleFire->AttachShader(fireSimpleShader);
	massParticles->AttachShader(starsSimpleShader);

	skyboxShader->Bind();
	skyboxShader->LoadProjectionMatrix(projection.GetProjectionMatrix());

	fireMassParticleShader->Bind();
	fireMassParticleShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
	fireMassParticleShader->LoadStaticTexture(fireTexture);
	fireMassParticleShader->LoadVec2(vec2(WIDTH, HEIGHT), "screenSize");
	fireMassParticleShader->LoadVec3(fireParticlePositions, "offsetPosition");

	starsMassParticleShader->Bind();
	starsMassParticleShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
	starsMassParticleShader->LoadStaticTexture(starTexture);
	starsMassParticleShader->LoadVec2(vec2(WIDTH, HEIGHT), "screenSize");
	starsMassParticleShader->LoadVec3(starsParticlePositions, "offsetPosition");

	fireSimpleShader->Bind();
	fireSimpleShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
	fireSimpleShader->LoadStaticTexture(fireTexture);
	
	starsSimpleShader->Bind();
	starsSimpleShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
	starsSimpleShader->LoadStaticTexture(starTexture);

	monkeyShader->Bind();
	monkeyShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
	
	for (auto& waterShader : waterShaders) {
		waterShader->Bind();
		waterShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
		waterShader->LoadStaticTexture(normalTexture);
		waterShader->LoadStaticTexture(dudvTexture);
		waterShader->LoadFloat(NEAR, "near");
		waterShader->LoadFloat(FAR, "far");
	}

	waterShader->Bind();
	waterShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
	waterShader->LoadLight(sunLight->GetPosition(), sunLight->GetColor(), sunLight->GetExponent(), sunLight->GetName());
	waterShader->LoadLight(fireLight->GetPosition(), fireLight->GetColor(), fireLight->GetExponent(), fireLight->GetName());
	waterShader->LoadStaticTexture(normalTexture);
	waterShader->LoadStaticTexture(dudvTexture);

	fboShader->Bind();
	fboShader->LoadProjectionMatrix(projection.GetProjectionMatrix());

	finalShader->Bind();
	finalShader->LoadProjectionMatrix(projection.GetProjectionMatrix());

	lightShader->Bind();
	lightShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
	lightShader->LoadStaticTexture(fireTexture);
	lightShader->LoadStaticTexture(dudvTexture);

	terrainShader->Bind();
	terrainShader->LoadProjectionMatrix(projection.GetProjectionMatrix());
	
	terrainShader->LoadStaticTexture(grassTexture);
	terrainShader->LoadStaticTexture(rockTexture);
	terrainShader->LoadStaticTexture(blendTexture);
	terrainShader->LoadStaticTexture(crackedTexture);
	terrainShader->LoadStaticTexture(terrainRockTexture);
	terrainShader->LoadLight(sunLight->GetPosition(), sunLight->GetColor(), sunLight->GetExponent(), sunLight->GetName());
	terrainShader->LoadLight(fireLight->GetPosition(), fireLight->GetColor(), fireLight->GetExponent(), fireLight->GetName());

	velocityShader->Bind();
	velocityShader->LoadStaticTexture(fireTexture);

	terrainEntity->AddFBO(lightDepthFBO);
	terrainEntity->AddFBO(lightDepthFBO2);
	terrainEntity->AddFBO(lightDepthFBO3);
	terrainEntity->AddFBO(lightDepthFBO4);
	terrainEntity->AddFBO(lightDepthFBO5);

	initVelocityShader->Bind();
	initVelocityShader->LoadVec2(vec2(WIDTH, HEIGHT), "screenSize");

	velocityShader->Bind();
	velocityShader->LoadVec2(vec2(WIDTH, HEIGHT), "screenSize");

	positionShader->Bind();
	positionShader->LoadVec2(vec2(WIDTH, HEIGHT), "screenSize");

	particleOffsetInfoShader->Bind();
	particleOffsetInfoShader->LoadVec2(vec2(WIDTH, HEIGHT), "screenSize");

	particleTexCoordInfoShader->Bind();
	particleTexCoordInfoShader->LoadVec2(vec2(WIDTH, HEIGHT), "screenSize");

	float counter = 0;
	float time;
	bool velocityToggle = true;
	bool firstFrame = true;

	sunLight->SetRendering(true);

	while (!display->IsClosed()) {

		display->ClearWindow(0.0f, 0.0f, 0.0f, 1.0f);
		
		if (firstFrame) {
			velocityFBO->Bind();
			glDisable(GL_BLEND);
			initVelocityShader->Bind();
			sunMesh->Draw();
			velocityFBO->Unbind();

			positionFBO->Bind();
			glDisable(GL_BLEND);
			initPositionShader->Bind();
			sunMesh->Draw();
			positionFBO->Unbind();

			firstFrame = false;
		}

		firstFrame = HandleEvents(display, skybox, camera, sunLight, fireLight, starsLight, simpleFire, massParticles, eventHolder, waterEntity, waterShaders);

		if (simpleFire->IsAnimating()) {
			simpleFire->UpdateBuffers(camera->GetForward(), 0.02f, fireLight);
		}

		skybox->UpdateBlendFactor(0.001);

		if (eventHolder->WaterMoving()) {
			for (auto& waterShader : waterShaders) {
				waterShader->Bind();
				waterShader->LoadFloat(counter, "t");
			}
		}

		lightShader->Bind();
		lightShader->LoadFloat(counter, "t");

		monkeyEntity->SetRotation(vec3(0, 500 * counter, 0));
		teddyEntity->SetRotation(vec3(0, 500 * counter, 0));
		
		if (eventHolder->RenderParticles()) {
			if (velocityToggle) {
				velocityFBO2->Bind();
				glDisable(GL_BLEND);
				velocityShader->Bind();
				velocityShader->LoadFBOTexture(velocityFBO);
				velocityShader->LoadFBOTexture(positionFBO);
				sunMesh->Draw();
				velocityFBO2->Unbind();

				positionFBO2->Bind();
				glDisable(GL_BLEND);
				positionShader->Bind();
				positionShader->LoadFBOTexture(velocityFBO2);
				positionShader->LoadFBOTexture(positionFBO);
				sunMesh->Draw();
				positionFBO2->Unbind();

				if (eventHolder->RenderFire()) {
					particleOffsetInfoFBO->Bind();
					glDisable(GL_BLEND);
					particleOffsetInfoShader->Bind();
					particleOffsetInfoShader->LoadFBOTexture(positionFBO2);
					sunMesh->Draw();
					particleOffsetInfoFBO->Unbind();


					particleTexCoordInfoFBO->Bind();
					glDisable(GL_BLEND);
					particleTexCoordInfoShader->Bind();
					particleTexCoordInfoShader->LoadFBOTexture(particleOffsetInfoFBO);
					sunMesh->Draw();
					particleTexCoordInfoFBO->Unbind();
				}

			}
			else {
				velocityFBO->Bind();
				glDisable(GL_BLEND);
				velocityShader->Bind();
				velocityShader->LoadFBOTexture(velocityFBO2);
				velocityShader->LoadFBOTexture(positionFBO2);
				sunMesh->Draw();
				velocityFBO->Unbind();

				positionFBO->Bind();
				glDisable(GL_BLEND);
				positionShader->Bind();
				positionShader->LoadFBOTexture(velocityFBO);
				positionShader->LoadFBOTexture(positionFBO2);
				sunMesh->Draw();
				positionFBO->Unbind();

				if (eventHolder->RenderFire()) {
					particleOffsetInfoFBO->Bind();
					glDisable(GL_BLEND);
					particleOffsetInfoShader->Bind();
					particleOffsetInfoShader->LoadFBOTexture(positionFBO);
					sunMesh->Draw();
					particleOffsetInfoFBO->Unbind();

					particleTexCoordInfoFBO->Bind();
					glDisable(GL_BLEND);
					particleTexCoordInfoShader->Bind();
					particleTexCoordInfoShader->LoadFBOTexture(particleOffsetInfoFBO);
					sunMesh->Draw();
					particleTexCoordInfoFBO->Unbind();
				}

			}
		}
		if (eventHolder->RenderReflection()) {
			renderer.RenderReflection(skybox, reflectionEntities, particlesList, camera, lights);
		}
		if (eventHolder->RenderRefraction()) {
			renderer.RenderRefraction(skybox, refractionEntities, camera, lights);
		}
		if (eventHolder->RenderBottomDepth()) {
			renderer.RenderBottomDepth(terrainEntity, camera->GetCameraMatrix(), projection.GetProjectionMatrix());
		}
		if (eventHolder->RenderShadows()) {
			renderer.RenderDepthMaps(reflectionEntities, lights);
		}

		if (eventHolder->RenderEnvironment()) {
			renderer.RenderEnvironment(skybox, environmentEntities, particlesList, camera, lights);
		}
		
		if (eventHolder->RenderReflectionFBO()) {
			showReflectionShader->Bind();
			showReflectionShader->LoadFBOTexture(reflectionFBO);
			sunMesh->Draw();
		}

		if (eventHolder->RenderRefractionFBO()) {
			showRefractionShader->Bind();
			showRefractionShader->LoadFBOTexture(refractionFBO);
			sunMesh->Draw();
		}

		if (eventHolder->RenderBottomDepthFBO()) {
			showWaterDepthShader->Bind();
			showWaterDepthShader->LoadFBOTexture(waterDepthFBO);
			sunMesh->Draw();
		}

		if (eventHolder->RenderShadowFBO()) {
			showLightDepthShader->Bind();
			showLightDepthShader->LoadFBOTexture(lightDepthFBO);
			sunMesh->Draw();
		}
		
		if (eventHolder->RenderParticles()) {
			if (eventHolder->RenderFire()) {
				if (velocityToggle) {
					fireMassParticleShader->Bind();
					fireMassParticleShader->LoadCameraMatrix(camera->GetCameraMatrix());
					fireMassParticleShader->LoadVec3(camera->GetForward(), "cameraForward");
					fireMassParticleShader->LoadFBOTexture(positionFBO2);
					fireMassParticleShader->LoadFBOTexture(particleTexCoordInfoFBO);
					fireMassParticleShader->LoadFBOTexture(particleOffsetInfoFBO);
					massParticles->Draw();
					velocityToggle = false;
				}
				else {
					fireMassParticleShader->Bind();
					fireMassParticleShader->LoadCameraMatrix(camera->GetCameraMatrix());
					fireMassParticleShader->LoadVec3(camera->GetForward(), "cameraForward");
					fireMassParticleShader->LoadFBOTexture(positionFBO);
					fireMassParticleShader->LoadFBOTexture(particleTexCoordInfoFBO);
					fireMassParticleShader->LoadFBOTexture(particleOffsetInfoFBO);
					massParticles->Draw();
					velocityToggle = true;
				}
			}
			else {

				if (velocityToggle) {
					starsMassParticleShader->Bind();
					starsMassParticleShader->LoadCameraMatrix(camera->GetCameraMatrix());
					starsMassParticleShader->LoadVec3(camera->GetForward(), "cameraForward");
					starsMassParticleShader->LoadFBOTexture(positionFBO2);
					massParticles->Draw();
					velocityToggle = false;
				}
				else {
					starsMassParticleShader->Bind();
					starsMassParticleShader->LoadCameraMatrix(camera->GetCameraMatrix());
					starsMassParticleShader->LoadVec3(camera->GetForward(), "cameraForward");
					starsMassParticleShader->LoadFBOTexture(positionFBO);
					massParticles->Draw();
					velocityToggle = true;
				}
			}
		}

		if (eventHolder->RenderParticleFBOS()) {

			if (velocityToggle) {
				showVelocityShader->Bind();
				showVelocityShader->LoadFBOTexture(velocityFBO2);
				sunMesh->Draw();

				showPositionShader->Bind();
				showPositionShader->LoadFBOTexture(positionFBO2);
				sunMesh->Draw();
			} else {

				showVelocityShader->Bind();
				showVelocityShader->LoadFBOTexture(velocityFBO);
				sunMesh->Draw();

				showPositionShader->Bind();
				showPositionShader->LoadFBOTexture(positionFBO);
				sunMesh->Draw();
			}

			if (eventHolder->RenderFireFBO()) {
				showOffsetShader->Bind();
				showOffsetShader->LoadFBOTexture(particleOffsetInfoFBO);
				sunMesh->Draw();
			}
		}

		/*
		finalShader->Bind();	
		glUniform1i(glGetUniformLocation(finalShader->GetProgram(), "environmentTex"), 21);
		Render(finalEntity, camera->GetCameraMatrix());*/

		display->UpdateWindow();

		counter = counter + 0.0002f;

	}

	return 0;
}