#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

struct Projection {

public:
	Projection(float fov, float aspect, float zNear, float zFar) {

		projection = perspective(fov, aspect, zNear, zFar);
	}

	inline mat4 GetProjectionMatrix() const {
		return projection;
	}

private:
	mat4 projection;
};

