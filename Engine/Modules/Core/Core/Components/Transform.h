#pragma once

#include <Core/Core.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Mani
{
	// todo #12: move transform component out of Core.
	struct Transform
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::mat4 calculate() const;
	
		glm::vec3 forward() const;
		glm::vec3 up() const;
		glm::vec3 right() const;

		glm::mat4 getTransform() const;

		void apply(const Transform& other);
	};
}