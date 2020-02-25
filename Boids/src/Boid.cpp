#pragma once
#include "Boid.h"
#include "glm.hpp"
#include "stdlib.h"
#include <time.h>

float genRandF() {
	return ((float)rand() / (RAND_MAX));
}

Boid::Boid(float x, float y, float angle, float speed, glm::vec4 colour)
	: angle(angle), speed(speed), colour(colour)
{
	position.x = x;
	position.y = y;
	velocity = glm::vec3(speed * cos(angle), speed * sin(angle), 0.0f);
	accelation = glm::vec3(0.0f, 0.00f, 0.0f);
}

Boid::Boid(){

	srand(time(NULL));

	//location = glm::vec2( ((float)rand() / (RAND_MAX)) ,  ((float)rand() / (RAND_MAX)));
	position = glm::vec3(0.0f, 0.0f, 0.0f); // Location
	velocity = glm::vec3(genRandF() * 0.01f, genRandF() * 0.01f, 1.0f); // Speed and direction
	accelation = glm::vec3(0.0f, 0.0f, 0.0f); // Change in direction or speed
	angle = (float)rand() * 3.1415;
	speed = 0.01;
	colour = glm::vec4(1.0f, 0.5f, 0.3f, 1.0);
}

void Boid::move() {

	//velocity.x = speed * cos(angle);
	//velocity.y = speed * sin(angle);
	velocity += accelation;
	position += velocity;

	if (position.x > 1.1f) {
		position.x = -1.0f;
	}
	else if (position.x < -1.1f) {
		position.x = 1.0f;
	}
	if (position.y > 1.1f) {
		position.y = -1.0f;
	}
	else if (position.y < -1.1f) {
		position.y = 1.0f;
	}
};

float Boid::getAngle() {
	//return glm::atan(velocity.y, velocity.x); // atan2, if (x < 0) return (result + PI)
}

float Boid::getX() {
	return position.x;
};
float Boid::getY() {
	return position.y;
};