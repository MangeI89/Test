#include "Particle.h"



Particle::Particle(const vec3& position, const vec3& velocity) {

	this->currentPosition = position;
	this->initialPosition = position;
	this->currentVelocity = velocity;
	this->initialVelocity = velocity;
}

Particle::~Particle() { }

void Particle::Reset() {
	currentPosition = initialPosition;
	currentVelocity = initialVelocity;
	lifeTime = 0;
}

void Particle::UpdatePosition(const float& deltaT) {

	currentPosition = currentPosition + currentVelocity * deltaT;
	if (currentPosition.y < 0) {
		Reset();
	}

	lifeTime += deltaT;
}

void Particle::UpdateVelocity(const float& deltaT) {
	currentVelocity.y = currentVelocity.y - 9.82 * deltaT;
}

float Particle::GetBlendFactor() {
	double fraction, intFactor;
	double mod = lifeTime / 0.125;

	fraction = modf(mod, &intFactor);
	return fraction;
}

vec2 Particle::GetOffset() {

	double xFraction1, yFraction1, xFraction2, yFraction2, xInt1, yInt1, xInt2, yInt2;
	double xMod = lifeTime / 0.125;
	double yMod = lifeTime / 0.5;

	xFraction1 = modf(xMod, &xInt1);
	yFraction1 = modf(yMod, &yInt1);

	xFraction2 = modf(xInt1 / 4.0, &xInt2);
	yFraction2 = modf(yInt1 / 4.0, &yInt2);

	return vec2(xInt1, yInt1);
}

vec3 Particle::GetPosition() { return currentPosition; }



