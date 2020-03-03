#pragma once
#include "Flock.h"
#include <vector>
#include "stdlib.h"

float genRandomFloat() {
	return ((float)rand() / (RAND_MAX));
}


Flock::Flock(const int flockSize)
{
	boids.reserve(flockSize);
	translations.reserve(flockSize);

	for (int i = 0; i < flockSize; i++) {
		boids.emplace_back(genRandomFloat(), genRandomFloat(),
			genRandomFloat() * 2.0f * 3.1415f, 0.01f, //genRandomFloat()*0.01f + 0.005f, // Random speed
			glm::vec4(genRandomFloat(), genRandomFloat(), genRandomFloat(), 1.0f));

		boids[i].setFlockMates(&boids);
		boids[i].interestPoint = &interestPoint;
		boids[i].interested = &interested;
		boids[i].timeModifier = timeModifier;
	}
}

void Flock::moveFlock(float &deltaTime) {
	for (int i = 0; i < getFlockSize(); i++) {
		boids[i].move(deltaTime);
	}
}

void Flock::addBoid(float x, float y, float maxSpeed, glm::vec4 colour) {
	boids.emplace_back(Boid(x, y, genRandomFloat() * 2 * 3.1415, maxSpeed, colour));
	for (int i = 0; i < getFlockSize(); i++) {
		boids[i].setFlockMates(&boids);
		boids[i].interestPoint = &interestPoint;
		boids[i].interested = &interested;
		boids[i].timeModifier = timeModifier;
	}

	translations.emplace_back(boids[boids.size()-1].getTransMatrix());
	reserveLeft -= 1;
}

int Flock::getFlockSize() {
	return boids.size();
}

std::vector<glm::mat4> Flock::getTranslations() {
	std::vector<glm::mat4> translations;
	translations.reserve(boids.size());
	for (int i = 0; i < getFlockSize(); i++) {
		translations.emplace_back(boids[i].getTransMatrix());
	}
	return translations;
}

std::vector<glm::vec4> Flock::getColours() {
	std::vector<glm::vec4> colours;
	colours.reserve(boids.size());
	for (int i = 0; i < getFlockSize(); i++) {
		colours.push_back(boids[i].colour);
	}
	return colours;
}

void Flock::resetReserve() {
	reserveLeft = RESERVE_SIZE;
}

void Flock::removeBoid() {
	if (getFlockSize() >=1) {
		boids.pop_back();
	}
}

void Flock::toggleInterested() {
	interested = !interested;
}

void Flock::changeTimeModifier(float amount) {
	timeModifier += amount;
	for (int i = 0; i < getFlockSize(); i++) {
		boids[i].changeTimeModifier(amount);
	}
}