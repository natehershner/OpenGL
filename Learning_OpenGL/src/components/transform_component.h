#pragma once
#include "../config.h"

//x, y, z and pitch, yaw, roll
struct TransformComponent {
	glm::vec3 position;
	glm::vec3 eulers;
};
