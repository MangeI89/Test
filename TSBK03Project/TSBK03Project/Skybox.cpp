#include "Skybox.h"
#include "stb_image.h"


Skybox::Skybox(const shared_ptr<Mesh>& mesh, const vec3& position, const vec3& rotation, const vec3& scale) {
	this->mesh = mesh;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;

	nrOfDaySkyboxTextures = 0;
	nrOfNightSkyboxTextures = 0;
	blendFactor = 0.0;
	dayMode = false;
}

void Skybox::AddSkyboxTextureSet(const string textureFileName[6], const string& dayOrNight) {

	int width, height, numComponents;
	GLuint skyboxTexture;
	glGenTextures(1, &skyboxTexture);			// Generate OpenGL texture IDs
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	for (int i = 0; i < 6; i++)
	{
		unsigned char* imageData = stbi_load(textureFileName[i].c_str(), &width, &height, &numComponents, 4);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	if (dayOrNight == "day") {
		daySkyboxTextures.push_back(skyboxTexture);
		nrOfDaySkyboxTextures++;
		currentDaySkyboxTexture = nrOfDaySkyboxTextures - 1;
	}
	else {
		nightSkyboxTextures.push_back(skyboxTexture);
		nrOfNightSkyboxTextures++;
		currentNightSkyboxTexture = nrOfNightSkyboxTextures - 1;
	}

	// MIPMAPPING
	//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Skybox::AttachShader(const shared_ptr<Shader>& shader) {
	this->shader = shader;
}

Skybox::~Skybox(){

}

void Skybox::ChooseDaySkybox(const GLuint& unit) {
	currentDaySkyboxTexture = daySkyboxTextures[unit];
}

void Skybox::ChooseNightSkybox(const GLuint& unit) {
	currentNightSkyboxTexture = nightSkyboxTextures[unit];
}

void Skybox::Bind() {

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, daySkyboxTextures[currentDaySkyboxTexture]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "daySkybox"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, nightSkyboxTextures[currentNightSkyboxTexture]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "nightSkybox"), 1);
}

shared_ptr<Shader> Skybox::GetShader() {
	return shader;
}

void Skybox::Draw() {

	shader->Bind();
	
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, daySkyboxTextures[currentDaySkyboxTexture]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "daySkybox"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, nightSkyboxTextures[currentNightSkyboxTexture]);
	glUniform1i(glGetUniformLocation(shader->GetProgram(), "nightSkybox"), 1);

	glUniform1f(glGetUniformLocation(shader->GetProgram(), "blendFactor"), blendFactor);

	mesh->Draw();

	glEnable(GL_DEPTH_TEST);
}

void Skybox::UpdateBlendFactor(const GLfloat& deltaT) {
	
	if (dayMode) {
		if (blendFactor < 1.0) {
			blendFactor += deltaT;
			blendFactor = min(blendFactor, 1.0f);
		}
	}
	else {
		if (blendFactor > 0.0) {
			blendFactor -= deltaT;
			blendFactor = max(blendFactor, 0.0f);
		}
	}
}

void Skybox::ToggleSkybox() {
	
	if(dayMode) { 
		dayMode = false;
	}
	else {
		dayMode = true;
	}
}

float Skybox::GetBlendFactor() {
	return blendFactor;
}