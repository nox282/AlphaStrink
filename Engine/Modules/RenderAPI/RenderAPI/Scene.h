#pragma once

#include <Core/Components/Transform.h>

namespace Mani
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