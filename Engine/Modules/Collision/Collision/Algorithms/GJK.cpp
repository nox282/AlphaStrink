#include <GJK.h>

using namespace Mani;

bool GJK::solve(const std::vector<glm::vec3>& shape1, const Transform* transform1, const std::vector<glm::vec3>& shape2, const Transform* transform2)
{
	constexpr size_t MAX_ITERATION = 32;
	MANI_ASSERT(shape1.size() > 1 && shape2.size() > 1, "Shapes cannot be empty");
	MANI_ASSERT(transform1 != nullptr && transform2 != nullptr, "Transforms cannot be nullptr");

	glm::mat4 matrix1 = transform1->calculateModelMatrix();
	glm::mat4 matrix2 = transform2->calculateModelMatrix();

	Simplex simplex;
	Simplex directions;
	glm::vec3 nextDirection;

	auto calculateSimplex = [&shape1, &matrix1, &shape2, &matrix2] (const glm::vec3& direction) -> glm::vec3
	{
		return  GJK::getSupport(shape2, matrix2, direction) - GJK::getSupport(shape1, matrix1, -direction);
	};

	//const glm::vec3 p1 = matrix1 * glm::vec4(shape1[0].x, shape1[0].y, shape1[0].z, 1.f);
	//const glm::vec3 p2 = matrix2 * glm::vec4(shape2[0].x, shape2[0].y, shape2[0].z, 1.f);

	//const glm::vec3 p0Delta = shape1[0] - shape2[0];
	//if (glm::length2(p0Delta) < FLT_EPSILON)
	//{
	//	directions.a = glm::vec3(0.f);
	//}
	//else
	//{
	//	directions.a = glm::normalize(p0Delta);
	//}

	directions.a = glm::vec3(1.f, 0.f, 0.f);
	directions.count = 1;

	simplex.a = calculateSimplex(directions.a);
	simplex.count = 1;

	if (glm::dot(simplex.a, directions.a) < 0)
	{
		// first support is opposed to origin, overlap is impossible.
		return false;
	}

	nextDirection = -simplex.a;

	for (size_t iteration = 0; iteration < MAX_ITERATION; iteration++)
	{
		directions.d = directions.c;
		directions.c = directions.b;
		directions.b = directions.a;
		directions.count++;

		simplex.d = simplex.c;
		simplex.c = simplex.b;
		simplex.b = simplex.a;
		simplex.count++;

		directions.a = nextDirection;
		simplex.a = calculateSimplex(directions.a);

		// exit condition
		if (glm::dot(simplex.a, directions.a) < 0)
		{
			// next support is opposed to origin, overlap is impossible.
			return false;
		}

		switch (simplex.count)
		{
			case 2:
			{
				// solve simplex for a and b.
				nextDirection = solveSimplex2(simplex, directions);
				break;
			}

			case 3:
			{
				// solve simplex for a, b, c.
				nextDirection = solveSimplex3(simplex, directions);
				break;
			}

			case 4:
			{
				// solve simplex for a, b, c, d.
				bool doesCollide = false;
				nextDirection = solveSimplex4(simplex, directions, doesCollide);
				if (doesCollide)
				{
					// collision found
					return true;
				}
				break;
			}

			default:
			{
				MANI_ASSERT(false, "Invalid simplex dimensions");
				break;
			}
		}
	}

	MANI_ASSERT(false, "We've reached max iteration, that should not happen.");
}

glm::vec3 GJK::getSupport(const std::vector<glm::vec3>& shape, const glm::mat4& transform, const glm::vec3& direction)
{
	const glm::vec3 transformedDirection = transformVertex(direction, transform);

	float maxDot = glm::dot(transformVertex(shape[0], transform), transformedDirection);
	size_t maxVertexIndex = 0;
	float currentDot = maxDot;
	for (size_t index = 1; index < shape.size(); index++)
	{
		currentDot = glm::dot(transformVertex(shape[index], transform), transformedDirection);
		if (currentDot > maxDot)
		{
			maxDot = currentDot;
			maxVertexIndex = index;
		}
	}

	return shape[maxVertexIndex];
}

glm::vec3 GJK::solveSimplex2(Simplex& simplex, Simplex& directions)
{
	const glm::vec3 ab = simplex.b - simplex.a;
	const glm::vec3 ao = -simplex.a;

	if (glm::dot(ab, ao) > 0)
	{
		// origin is on the side of ab, we continue.
		return glm::cross(glm::cross(ab, ao), ab);
	}
	else
	{
		// a is closest to origin
		simplex.count = 1;
		directions.count = 1;
		return ao;
	}


	return glm::vec3();
}

glm::vec3 GJK::solveSimplex3(Simplex& simplex, Simplex& directions)
{
	const glm::vec3 ao = -simplex.a;

	const glm::vec3 abc = glm::cross(simplex.b - simplex.c, simplex.a - simplex.c);
	const glm::vec3 ac = simplex.c - simplex.a;

	if (glm::dot(glm::cross(abc, ac), ao) > 0)
	{
		if (glm::dot(ac, ao) > 0)
		{
			// ac is closest to origin
			simplex.b = simplex.c;
			simplex.count = 2;

			directions.b = directions.c;
			directions.count = 2;
			return glm::cross(glm::cross(ac, ao), ac);
		}
		else
		{
			const glm::vec3 ab = simplex.b - simplex.a;
			if (glm::dot(ab, ao) > 0)
			{
				// ab is closest to origin, no need to swap points.
				simplex.count = 2;
				directions.count = 2;
				return glm::cross(glm::cross(ab, ao), ab);
			}
			else
			{
				// a is closest to origin
				simplex.count = 1;
				directions.count = 1;
				return ao;
			}
		}
	}
	else
	{
		const glm::vec3 ab = simplex.b - simplex.a;
		if (glm::dot(glm::cross(ab, abc), ao) > 0)
		{
			const glm::vec3 ab = simplex.b - simplex.a;
			if (glm::dot(ab, ao) > 0)
			{
				// ab is closest to origin, no need to swap points.
				simplex.count = 2;
				directions.count = 2;
				return glm::cross(glm::cross(ab, ao), ab);
			}
			else
			{
				// a is closest to origin
				simplex.count = 1;
				directions.count = 1;
				return ao;
			}
		}
		else
		{
			if (glm::dot(abc, ao) > 0)
			{
				return abc;
			}
			else
			{
				// acb is closest to origin
				// swap b and c
				simplex.d = simplex.b;
				simplex.b = simplex.c;
				simplex.c = simplex.d;

				directions.d = directions.b;
				directions.b = directions.c;
				directions.c = directions.d;

				return -abc;
			}
		}
	}
}

glm::vec3 GJK::solveSimplex4(Simplex& simplex, Simplex& directions, bool& doesCollide)
{
	doesCollide = false;
	const glm::vec3 ao = -simplex.a;
	
	const glm::vec3 abc = glm::cross(simplex.b - simplex.a, simplex.c - simplex.a);

	if (glm::dot(abc, ao) > 0)
	{
		// triangle abc is facing origin
		simplex.count = 3;
		directions.count = 3;

		// find a new 4th support point.
		return solveSimplex3(simplex, directions);
	}

	const glm::vec3 acd = glm::cross(simplex.c - simplex.a, simplex.d - simplex.c);
	
	if (glm::dot(acd, ao) > 0)
	{
		// triangle acd is facing origin
		simplex.b = simplex.c;
		simplex.c = simplex.d;
		simplex.count = 3;

		directions.b = directions.c;
		directions.c = directions.d;
		directions.count = 3;

		// find a new 4th support point.
		return solveSimplex3(simplex, directions);
	}

	const glm::vec3 adb = glm::cross(simplex.d - simplex.a, simplex.b - simplex.a);

	if (glm::dot(adb, ao) > 0)
	{
		// triangle abd is facing origin
		simplex.b = simplex.d;
		simplex.c = simplex.b;
		simplex.count = 3;

		directions.b = directions.d;
		directions.c = directions.b;
		directions.count = 3;

		// find a new 4th support point.
		return solveSimplex3(simplex, directions);
	}

	const glm::vec3 bdc = glm::cross(simplex.d - simplex.b, simplex.c - simplex.b);

	if (glm::dot(bdc, ao) > 0)
	{
		// triangle abd is facing origin
		simplex.a = simplex.b;
		simplex.b = simplex.d;
		simplex.count = 3;

		directions.a = directions.b;
		directions.b = directions.d;
		directions.count = 3;

		// find a new 4th support point.
		return solveSimplex3(simplex, directions);
	}

	doesCollide = true;
	return glm::vec3();
}

glm::vec3 Mani::GJK::transformVertex(const glm::vec3& vertex, const glm::mat4& transform)
{
	return transform * glm::vec4(vertex.x, vertex.y, vertex.z, 1.f);
}
