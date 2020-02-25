#pragma once
#include "Boid.h"
#include <vector>

class Flock {
public:
	int numOfBoids;
	std::vector<Boid> boids;

	Flock(int flockSize = 30);

	void moveFlock();
};