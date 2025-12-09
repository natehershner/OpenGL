#pragma once
#include "../config.h"

//Linear and angular velocity
struct PhysicsComponent {
	glm::vec3 velocity;
	glm::vec3 eulerVelocity;
};
