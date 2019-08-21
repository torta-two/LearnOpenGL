#pragma once
#include <GLM/glm.hpp>

class Object
{
public:
	Object();
	glm::mat4 GetTransform();
	void SetTransform(glm::mat4 trans);
	glm::vec3 position();
	glm::vec3 rotation();
	glm::vec3 scale();

	~Object();

private:
	glm::mat4 Transform;
};

