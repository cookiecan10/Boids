#pragma once
#include "Boid.h"
#include <vector>
#include "glm.hpp"

class Flock {
public:
	int numOfBoids;
	std::vector<Boid> boids;
	std::vector<glm::mat4> translations;

	Flock(const int flockSize = 30);

	void moveFlock();
	int getFlockSize();
	void addBoid(float x = 0.0f, float y = 0.0f, float maxSpeed = 0.01f, glm::vec4 colour = glm::vec4(1.0f, .5f, .3f, 1.0f));
	std::vector<glm::mat4> getTranslations();
	std::vector<glm::vec4> getColours();
};