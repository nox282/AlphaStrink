#include "simpleTests.h"
#include <Core/CoreFwd.h>
#include <Collision/Algorithms/GJK.h>

extern "C" __declspec(dllexport) void runTests()
{
	SimpleTests::SimpleTestsRunner::runTests();
}

using namespace Mani;

ST_SECTION_BEGIN(Collision, "Collision")
{
	ST_SECTION_BEGIN(GJKTest, "GJK")
	{
		ST_TEST(SimpleNoCollision, "simple case with no collision")
		{
			std::vector<glm::vec3> shape1 = {
				{ 0.f - .5f, 0.f - .5f, 0.f - .5f },
				{ 1.f - .5f, 0.f - .5f, 0.f - .5f },
				{ 1.f - .5f, 1.f - .5f, 0.f - .5f },
				{ 0.f - .5f, 1.f - .5f, 0.f - .5f },

				{ 0.f - .5f, 0.f - .5f, 1.f - .5f },
				{ 1.f - .5f, 0.f - .5f, 1.f - .5f },
				{ 1.f - .5f, 1.f - .5f, 1.f - .5f },
				{ 0.f - .5f, 1.f - .5f, 1.f - .5f }
			};

			Transform transform1;
			transform1.position = glm::vec3(1.f);

			std::vector<glm::vec3> shape2 = {
				{ 0.f - .5f, 0.f - .5f, 0.f - .5f },
				{ 1.f - .5f, 0.f - .5f, 0.f - .5f },
				{ 1.f - .5f, 1.f - .5f, 0.f - .5f },
				{ 0.f - .5f, 1.f - .5f, 0.f - .5f },

				{ 0.f - .5f, 0.f - .5f, 1.f - .5f },
				{ 1.f - .5f, 0.f - .5f, 1.f - .5f },
				{ 1.f - .5f, 1.f - .5f, 1.f - .5f },
				{ 0.f - .5f, 1.f - .5f, 1.f - .5f }
			};
			Transform transform2;
			transform2.position = glm::vec3(50.f);

			const bool result = GJK::solve(shape1, &transform1, shape2, &transform2);
			ST_ASSERT(result == false, "Shape aren't colliding");
		}

		ST_TEST(SimpleYesCollision, "simple case with collision")
		{
			std::vector<glm::vec3> shape1 = {
				{ 0.f - .5f, 0.f - .5f, 0.f - .5f },
				{ 1.f - .5f, 0.f - .5f, 0.f - .5f },
				{ 1.f - .5f, 1.f - .5f, 0.f - .5f },
				{ 0.f - .5f, 1.f - .5f, 0.f - .5f },

				{ 0.f - .5f, 0.f - .5f, 1.f - .5f },
				{ 1.f - .5f, 0.f - .5f, 1.f - .5f },
				{ 1.f - .5f, 1.f - .5f, 1.f - .5f },
				{ 0.f - .5f, 1.f - .5f, 1.f - .5f }
			};
			Transform transform1;
			transform1.position = glm::vec3(1.f);

			std::vector<glm::vec3> shape2 = {
				{ 0.f - .5f, 0.f - .5f, 0.f - .5f },
				{ 1.f - .5f, 0.f - .5f, 0.f - .5f },
				{ 1.f - .5f, 1.f - .5f, 0.f - .5f },
				{ 0.f - .5f, 1.f - .5f, 0.f - .5f },

				{ 0.f - .5f, 0.f - .5f, 1.f - .5f },
				{ 1.f - .5f, 0.f - .5f, 1.f - .5f },
				{ 1.f - .5f, 1.f - .5f, 1.f - .5f },
				{ 0.f - .5f, 1.f - .5f, 1.f - .5f }
			};

			Transform transform2;
			transform2.position = glm::vec3(1.5f);

			const bool result = GJK::solve(shape1, &transform1, shape2, &transform2);
			ST_ASSERT(result == true, "Shape are colliding");
		}
	}
	ST_SECTION_END(GJKTest)
}
ST_SECTION_END(Collision)