#pragma once
#include <glm.hpp>
#include <vector>
#include "glm.hpp"

class Boid {
private:
	glm::vec3 separation();
	glm::vec3 cohesion();
	glm::vec3 alignment();
	glm::vec3 interest();
	void clampVec(glm::vec3&, float c);
public:
	float angle;
	float maxSpeed;
	float timeModifier = 70.0f;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 accelation;
	glm::vec4 colour;

	bool* interested;
	glm::vec3* interestPoint;

	std::vector<Boid> *flockMates;

	Boid(float xPos, float yPos, float angle = 0.0f, float maxSpeed = 0.01f, glm::vec4 colour = glm::vec4(1.0f, 0.5f, 0.3f, 1.0));
	Boid();

	void move(float &deltaTime);
	float getAngle();
	float getX();
	float getY();
	void setFlockMates(std::vector<Boid>* flock);
	glm::mat4 getTransMatrix();
	void changeTimeModifier(float amount);
};