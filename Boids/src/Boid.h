#pragma once
#include <glm.hpp>

class Boid {
public:
	/*glm::vec2 location*/;
	float x;
	float y;
	float rotation;
	float speed;

	Boid(float xPos, float yPos, float rotation, float speed);
	Boid();

	void move();
};