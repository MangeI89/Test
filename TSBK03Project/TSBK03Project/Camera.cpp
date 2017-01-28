#include "Camera.h"

Camera::Camera(const vec3& pos) {

	position = pos;
	forward = vec3(0, 0, -1);
	up = vec3(0, 1, 0);

}

Camera::Camera(const vec3& pos, const vec3& target) {

	position = pos;
	forward = normalize(target - pos);
	up = vec3(0, 1, 0);

}

Camera::~Camera() {

}

mat4 Camera::GetCameraMatrix() {
	return lookAt(position, position + forward, up);
}

mat4 Camera::GetRotationMatrix() {
	mat4 cameraMatrix = lookAt(position, position + forward, up);
	cameraMatrix[3] = vec4(0.0, 0.0, 0.0, 1.0);
	return cameraMatrix;
}

mat4 Camera::GetTranslationMatrix() {
	mat4 cameraMatrix = lookAt(position, position + forward, up);
	cameraMatrix[0] = vec4(1.0, 0.0, 0.0, 0.0);
	cameraMatrix[1] = vec4(0.0, 1.0, 0.0, 0.0);
	cameraMatrix[2] = vec4(0.0, 0.0, 1.0, 0.0);

	return cameraMatrix;
}

vec3 Camera::GetPosition() {
	return position;
}

vec3 Camera::GetForward() {
	return forward;
}

void Camera::UpdateForward(GLfloat xOffset, GLfloat yOffset) {

	lastTime = currentTime;
	currentTime = SDL_GetTicks();
	GLfloat deltaT = currentTime - lastTime;

	mouseSpeed = 0.005 * deltaT;

	yaw += mouseSpeed * xOffset;
	pitch -= mouseSpeed * yOffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}

	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	forward.x = sin(radians(yaw)) * cos(radians(pitch));
	forward.y = sin(radians(pitch));
	forward.z = -cos(radians(yaw)) * cos(radians(pitch));

	forward = normalize(forward);

}

void Camera::MoveForward() {
	position += moveSpeed * forward;
}

void Camera::MoveBackwards() {
	position -= moveSpeed * forward;
}

void Camera::MoveLeft() {
	position -= normalize(cross(forward, up)) * moveSpeed;
}

void Camera::MoveRight() {
	position += normalize(cross(forward, up)) * moveSpeed;
}
