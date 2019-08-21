#include "Object.h"


Object::Object()
{
	Transform = glm::mat4(1.0f);
}

glm::mat4 Object::GetTransform()
{
	return Transform;
}

void Object::SetTransform(glm::mat4 trans)
{
	Transform = trans;
}

glm::vec3 Object::position()
{
	return glm::vec3();
}

glm::vec3 Object::rotation()
{
	return glm::vec3();
}

glm::vec3 Object::scale()
{
	return glm::vec3();
}

Object::~Object()
{
}
