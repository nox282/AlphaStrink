#pragma once

#include <Core/Components/Transform.h>

namespace ManiEngine
{
	struct Node
	{
		std::string meshPath;
		Transform transform;
	};

	struct Scene
	{
		Transform transform;
	};
}