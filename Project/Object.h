#pragma once
#include <GLM/glm.hpp>
#include "Transform.h"

class Object
{
public:
	Object();
	Transform *transform;

	~Object();
};

