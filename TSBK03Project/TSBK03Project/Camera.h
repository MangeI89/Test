#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;
using namespace glm;

class Camera {

public:
	Camera(const vec3& pos);
	Camera(const vec3& pos, const vec3& target);

	mat4 GetCameraMatrix();
	mat4 GetRotationMatrix();
	mat4 GetTranslationMatrix();
	vec3 GetPosition();
	vec3 GetForward();

	void UpdateForward(GLfloat xOffset, GLfloat yOffset);
	void MoveForward();
	void MoveBackwards();
	void MoveLeft();
	void MoveRight();

	~Camera();

private:

	vec3 position;
	vec3 forward;
	vec3 up;
	GLfloat yaw = 0;
	GLfloat pitch = 0;
	GLfloat mouseSpeed = 0;
	GLfloat moveSpeed = 2;
	GLfloat lastTime = 0;
	GLfloat currentTime = 0;

};