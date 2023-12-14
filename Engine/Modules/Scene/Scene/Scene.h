#pragma once

#include <Core/Interfaces/IJsonAsset.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <filesystem>
#include <string_view>

namespace Mani
{
    const std::string_view LogScene = "Scene";

	struct Scene : public IJsonAsset
	{
		struct Node
		{
			size_t id = UINT64_MAX;

			std::filesystem::path meshAsset;

			size_t parent = UINT64_MAX;

			glm::vec3 localPosition = glm::vec3(0.f);
			glm::quat localRotation = glm::quat(1.0f, 0.f, 0.f, 0.f);
			glm::vec3 localScale = glm::vec3(0.f);

			bool hasParent() const;
		};

		std::vector<Node> nodes;
		size_t root = UINT64_MAX;

		virtual void parse(const std::string_view& content) override;
		virtual std::string toJson() override;
	};
}