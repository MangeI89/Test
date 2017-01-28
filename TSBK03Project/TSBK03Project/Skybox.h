#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include "Shader.h"
#include "Mesh.h"

using namespace std;
using namespace glm;

class Skybox
{
public:

	Skybox(const shared_ptr<Mesh>& mesh, const vec3& position, const vec3& rotation, const vec3& scale);

	void AddSkyboxTextureSet(const string textureFileName[6], const string& dayOrNight);
	void AttachShader(const shared_ptr<Shader>& shader);
	void ChooseDaySkybox(const GLuint& unit);
	void ChooseNightSkybox(const GLuint& unit);
	void Bind();
	shared_ptr<Shader> GetShader();
	void UpdateBlendFactor(const GLfloat& deltaT);
	void Draw();
	void ToggleSkybox();
	float GetBlendFactor();

	~Skybox();

private:

	shared_ptr<Shader> shader;
	shared_ptr<Mesh> mesh;
	vector<GLuint> daySkyboxTextures;
	vector<GLuint> nightSkyboxTextures;
	GLuint nrOfDaySkyboxTextures;
	GLuint nrOfNightSkyboxTextures;
	GLuint currentDaySkyboxTexture;
	GLuint currentNightSkyboxTexture;
	vec3 position;
	vec3 rotation;
	vec3 scale;
	GLfloat blendFactor;
	bool dayMode;
	//string skyboxName;
};

