#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Mani;

glm::mat4 Transform::calculate() const
{
	return	glm::translate(glm::mat4(1.0f), position) *
			glm::toMat4(rotation) *
			glm::scale(glm::mat4(1.0f), scale);
}

glm::vec3 Transform::forward() const
{
	glm::vec3 forward = glm::rotate(rotation, glm::vec3(0.f, 0.f, 1.f));
	return glm::normalize(forward);
}

glm::vec3 Transform::up() const
{
	glm::vec3 up = glm::rotate(rotation, glm::vec3(0.f, 1.f, 0.f));
	return glm::normalize(up);
}

glm::vec3 Transform::right() const
{
	glm::vec3 right = glm::rotate(rotation, glm::vec3(1.f, 0.f, 0.f));
	return glm::normalize(right);
}

void Mani::Transform::apply(const Transform& other)
{
	glm::mat4 otherMat4 = other.calculate();
	glm::mat4 mat4 = calculate();
	glm::mat4 result = otherMat4 * mat4;

	position = result[3];

	rotation *= other.rotation;

	scale.x = glm::length(glm::vec3(result[0]));
	scale.y = glm::length(glm::vec3(result[1]));
	scale.z = glm::length(glm::vec3(result[2]));
}