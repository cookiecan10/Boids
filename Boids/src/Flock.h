#pragma once
#include "Boid.h"
#include <vector>
#include "glm.hpp"

class Flock {
public:
	int RESERVE_SIZE = 10000;
	int reserveLeft = RESERVE_SIZE;
	std::vector<Boid> boids;
	std::vector<glm::mat4> translations;
	glm::vec3 interestPoint;
	bool interested = false;
	float timeModifier = 70.0f;

	Flock(const int flockSize = 30);

	void moveFlock(float &deltaTime);
	int getFlockSize();
	void addBoid(float x = 0.0f, float y = 0.0f, float maxSpeed = 0.01f, glm::vec4 colour = glm::vec4(1.0f, .5f, .3f, 1.0f));
	void removeBoid();
	std::vector<glm::mat4> getTranslations();
	std::vector<glm::vec4> getColours();
	void resetReserve();
	void toggleInterested();
	void changeTimeModifier(float amount);
};