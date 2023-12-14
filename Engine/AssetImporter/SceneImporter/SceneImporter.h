#pragma once

#include <Scene/Scene.h>
#include <filesystem>
#include <memory>
#include <assimp/matrix4x4.h>

struct aiNode;
struct aiScene;

namespace Mani
{
	const std::string_view LogSceneImporter = "SceneImporter";

	struct Mesh;

	class SceneImporter
	{
	public:
		static bool importFromPath(const std::filesystem::path& path, const std::shared_ptr<Scene>& outScene, const std::vector<std::shared_ptr<Mesh>>& meshes);
		static bool exportToPath(const std::filesystem::path& path, const std::shared_ptr<Scene>& scene);

	private:
		static void processNode(aiNode* aiNode, const aiScene* aiScene, size_t parent, const std::filesystem::path path, const std::shared_ptr<Scene>& outScene);
		static size_t makeNode(const aiMatrix4x4& aiMatrix, const std::shared_ptr<Scene>& scene);
	};
}