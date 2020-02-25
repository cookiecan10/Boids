#pragma once
#include <glm.hpp>

class Boid {
public:
	/*glm::vec2 location*/;
	float angle;
	float speed;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 accelation;
	glm::vec4 colour;

	Boid(float xPos, float yPos, float angle, float speed, glm::vec4 colour = glm::vec4(1.0f, 0.5f, 0.3f, 1.0));
	Boid();

	void move();
	float getAngle();
	float getX();
	float getY();
};