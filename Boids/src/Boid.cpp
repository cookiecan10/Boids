#pragma once
#include "Boid.h"
#include "glm.hpp"
#include "stdlib.h"
#include <time.h>

Boid::Boid(float x, float y, float rotation, float speed)
	: x(x), y(y), rotation(rotation), speed(speed)
{

}

Boid::Boid(){

	srand(time(NULL));

	//location = glm::vec2( ((float)rand() / (RAND_MAX)) ,  ((float)rand() / (RAND_MAX)));
	x = .5f; // ((float)rand() / (RAND_MAX));
	y = .5f; // ((float)rand() / (RAND_MAX));
	rotation = (float)rand() * 3.1415;
	speed = 0.01;
}

void Boid::move() {

	x += speed * cos(rotation);
	y += speed * sin(rotation);

	if (x > 1.1f) {
		x = -1.0f;
	}
	else if (x < -1.1f) {
		x = 1.0f;
	}
	if (y > 1.1f) {
		y = -1.0f;
	}
	else if (y < -1.1f) {
		y = 1.0f;
	}
};