#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class Transform
{
public:
	Transform();
	glm::mat4 GetMatrix();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	~Transform();
};

