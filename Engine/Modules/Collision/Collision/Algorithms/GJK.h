#pragma once

#include <vector>
#include <glm/fwd.hpp>
#include <Core/CoreFwd.h>

namespace Mani
{
	class GJK
	{
	public:
		static bool solve(const std::vector<glm::vec3>& shape1, const Transform* transform1, const std::vector<glm::vec3>& shape2, const Transform* transform2);

	private:
		struct Simplex
		{
			glm::vec3 a;
			glm::vec3 b;
			glm::vec3 c;
			glm::vec3 d;

			unsigned char count = 0;
		};

		static glm::vec3 getSupport(const std::vector<glm::vec3>& shape, const glm::mat4& transform, const glm::vec3& direction);
		static glm::vec3 solveSimplex2(Simplex& simplex, Simplex& directions);
		static glm::vec3 solveSimplex3(Simplex& simplex, Simplex& directions);
		static glm::vec3 solveSimplex4(Simplex& simplex, Simplex& directions, bool& doesCollide);

		static glm::vec3 transformVertex(const glm::vec3& vertex, const glm::mat4& transform);
	};
}