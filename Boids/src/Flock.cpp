#pragma once
#include "Flock.h"
#include <vector>
#include "stdlib.h"

float genRandomFloat() {
	return ((float)rand() / (RAND_MAX));
}


Flock::Flock(int flockSize)
	: numOfBoids(flockSize)
{
	boids.reserve(numOfBoids);

	for (int i = 0; i < numOfBoids; i++) {
		boids.emplace_back(genRandomFloat(), genRandomFloat(),
			genRandomFloat() * 2.0f * 3.1415f, genRandomFloat()*0.01f + 0.005, // Random speed
			glm::vec4(genRandomFloat(), genRandomFloat(), genRandomFloat(), 1.0f));
	}
}

void Flock::moveFlock() {
	for (int i = 0; i < numOfBoids; i++) {
		boids[i].move();
	}
}