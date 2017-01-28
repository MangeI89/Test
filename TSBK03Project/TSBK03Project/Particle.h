#pragma once
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

class Particle
{
public:
	Particle(const vec3& position, const vec3& velocity);
	~Particle();

	void Reset();
	void UpdatePosition(const float& deltaT);
	void UpdateVelocity(const float& deltaT);
	float GetBlendFactor();
	vec2 GetOffset();
	vec3 GetPosition();

private:
	vec3 currentPosition, initialPosition;
	vec3 currentVelocity, initialVelocity;
	float lifeTime;
	bool resetWhenZero;
};
