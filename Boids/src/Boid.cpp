#pragma once
#include "Boid.h"
#include "glm.hpp"
#include "stdlib.h"
#include <time.h>
#include <vector>
#include <gtc/matrix_transform.hpp>

float genRandF() {
	return ((float)rand() / (RAND_MAX));
}

Boid::Boid(float x, float y, float angle, float maxSpeed, glm::vec4 colour)
	: angle(angle), maxSpeed(maxSpeed), colour(colour)
{
	position.x = x;
	position.y = y;
	position.z = 0.0f;
	velocity = glm::vec3(maxSpeed * cos(angle), maxSpeed * sin(angle), 0.0f);
	accelation = glm::vec3(0.0f, 0.00f, 0.0f);
}

Boid::Boid(){

	srand(time(NULL));

	//location = glm::vec2( ((float)rand() / (RAND_MAX)) ,  ((float)rand() / (RAND_MAX)));
	position = glm::vec3(0.0f, 0.0f, 0.0f); // Location
	velocity = glm::vec3(genRandF() * 0.01f, genRandF() * 0.01f, 0.0f); // Speed and direction
	accelation = glm::vec3(0.0f, 0.0f, 0.0f); // Change in direction or speed
	angle = (float)rand() * 3.1415;
	maxSpeed = 0.01;
	colour = glm::vec4(1.0f, 0.5f, 0.3f, 1.0f);
}

void Boid::move() {

	//velocity.x = speed * cos(angle);
	//velocity.y = speed * sin(angle);

	velocity += separation() * 0.02f;
	clampVec(velocity, maxSpeed);

	velocity += cohesion() * 0.0005f;
	clampVec(velocity, maxSpeed);

	velocity += alignment() * 0.05f;
	clampVec(velocity, maxSpeed);

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
	return glm::atan(velocity.y, velocity.x); // atan2, if (x < 0) return (result + PI)
}

float Boid::getX() {
	return position.x;
};
float Boid::getY() {
	return position.y;
};

glm::vec3 Boid::separation() {
	auto separation = glm::vec3(0.0f, 0.0f, 0.0f);

	const float careDistance = 0.15f;

	float distance;
	for (int i = 0; i < flockMates->size(); i++) {
		distance = glm::length(position - (*flockMates)[i].position);
		if (distance < 0.15f) {
			// Care more about nearby flockmates
			separation += (position - (*flockMates)[i].position) * ((careDistance - distance) / careDistance);
		}
	}
	return separation;
}

glm::vec3 Boid::cohesion() {
	auto averagePos = glm::vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < flockMates->size(); i++) {
		if (glm::length(position - (*flockMates)[i].position) < 0.5f) {
			averagePos += (*flockMates)[i].position;
		}
	}
	averagePos /= flockMates->size();
	return averagePos - position;
}

glm::vec3 Boid::alignment() {
	auto averageAlignment = glm::vec3(0.0f, 0.0f, 0.0f);
	int count = 0;
	for (int i = 0; i < flockMates->size(); i++) {
		if (glm::length(position - (*flockMates)[i].position) < 0.5f) {
			averageAlignment += (*flockMates)[i].velocity;
			count++;
		}
	}
	averageAlignment /= count;
	return averageAlignment;
}

void Boid::setFlockMates(std::vector<Boid>* flock) {
	flockMates = flock;
}

void Boid::getTransMatrix(glm::mat4 &trans) {
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(getX(), getY(), 0.0f));
	trans = glm::rotate(trans, getAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));
}

void Boid::clampVec(glm::vec3 &vec, float c = 0.01f) {
	if (glm::length(vec) > 0.01f) {
		vec = glm::normalize(vec) * c;
	}
}