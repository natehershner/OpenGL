#pragma once
#include "../config.h"

//Basis of camera's coordinate system
struct CameraComponent {
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 forwards;
};
