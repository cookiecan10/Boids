#pragma once
#include "Flock.h"
#include <vector>
#include "stdlib.h"

float genRandomFloat() {
	return ((float)rand() / (RAND_MAX));
}


Flock::Flock(const int flockSize)
{
	boids.reserve(flockSize+100);
	translations = new glm::mat4[flockSize];

	for (int i = 0; i < flockSize; i++) {
		boids.emplace_back(genRandomFloat(), genRandomFloat(),
			genRandomFloat() * 2.0f * 3.1415f, 0.01f, //genRandomFloat()*0.01f + 0.005f, // Random speed
			glm::vec4(genRandomFloat(), genRandomFloat(), genRandomFloat(), 1.0f));

		boids[i].setFlockMates(&boids);
	}
}

void Flock::moveFlock() {
	for (int i = 0; i < getFlockSize(); i++) {
		boids[i].move();
	}
}

void Flock::addBoid(float x, float y, float maxSpeed, glm::vec4 colour) {
	boids.emplace_back(Boid(x, y, genRandomFloat() * 2 * 3.1415, maxSpeed, colour));
	for (int i = 0; i < getFlockSize(); i++) {
		boids[i].setFlockMates(&boids);
	}
	translations = new glm::mat4[getFlockSize()];
}

int Flock::getFlockSize() {
	return boids.size();
}

glm::mat4* Flock::getTranslations() {
	for (int i = 0; i < getFlockSize(); i++) {
		boids[i].getTransMatrix(translations[i]);
	}
	return translations;
}

glm::vec4* Flock::getColours() {
	glm::vec4* colours = new glm::vec4[getFlockSize()];
	for (int i = 0; i < getFlockSize(); i++) {
		colours[i] = boids[i].colour;
	}
	return colours;
}