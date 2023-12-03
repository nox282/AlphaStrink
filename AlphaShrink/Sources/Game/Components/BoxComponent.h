#pragma once

#include <glm/glm.hpp>
#include <Core/Components/Transform.h>

struct BoxComponent
{
	glm::vec3 extent = glm::vec3(1.f, 1.f, 1.f);

	static void getBounds(const BoxComponent& boxComponent, const Mani::Transform& boxTransform, glm::vec3& outMin, glm::vec3& outMax);
	static bool isIn(const BoxComponent& boxComponent, const Mani::Transform& boxTransform, const glm::vec3& position);
};