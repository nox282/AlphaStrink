#include "SceneImporter.h"

#include <Core/Log.h>
#include <Core/FileSystem.h>
#include <Core/ManiAssert.h>

#include <RenderAPI/Mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Mani;
namespace fs = std::filesystem;

bool SceneImporter::importFromPath(const fs::path& path, const std::shared_ptr<Scene>& outScene, const std::vector<std::shared_ptr<Mesh>>& meshes)
{
	Assimp::Importer importer;
	const aiScene* aiScene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (aiScene == nullptr || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || aiScene->mRootNode == NULL)
	{
		MANI_LOG_ERROR(LogSceneImporter, "Could not load scene: {}", std::string_view(importer.GetErrorString()));
		return false;
	}

	Scene::Node rootNode;
	rootNode.id = 0;
	outScene->root = rootNode.id;
	outScene->nodes.push_back(rootNode);

	// get the path of the parent directory
	const fs::path parentPath = path.parent_path();

	fs::path rootPath;
	if (!FileSystem::tryGetRootPath(rootPath))
	{
		MANI_LOG_ERROR(LogSceneImporter, "Could not get root path");
		return false;
	}

	// transform the parent directory path to a relative path
	const fs::path relativePath = fs::relative(parentPath, rootPath);

	processNode(aiScene->mRootNode, aiScene, 0, relativePath, outScene);

	return true;
}

bool Mani::SceneImporter::exportToPath(const fs::path& path, const std::shared_ptr<Scene>& scene)
{
	MANI_ASSERT(scene != nullptr, "provided scene cannot be null");

	return FileSystem::tryWriteFile(path, scene->toJson());
}

void SceneImporter::processNode(aiNode* aiNode, const aiScene* aiScene, size_t parent, const fs::path path, const std::shared_ptr<Scene>& outScene)
{
	if (aiNode == nullptr)
	{
		return;
	}

	size_t nodeId = makeNode(aiNode->mTransformation, outScene);
	outScene->nodes[nodeId].parent = parent;
	
	for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
	{
		const aiMesh* aiMesh = aiScene->mMeshes[aiNode->mMeshes[i]];
		size_t meshNodeId = makeNode(aiNode->mTransformation, outScene);
		Scene::Node& meshNode = outScene->nodes[meshNodeId];
		meshNode.parent = nodeId;
		meshNode.meshAsset = path;
		meshNode.meshAsset.append(std::format("{}.mesh", aiMesh->mName.C_Str()));
	}

	for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
	{
		processNode(aiNode->mChildren[i], aiScene, nodeId, path, outScene);
	}
}

size_t SceneImporter::makeNode(const aiMatrix4x4& aiMatrix, const std::shared_ptr<Scene>& scene)
{
	aiVector3t<float> aiScaling;
	aiQuaterniont<float> aiRotation;
	aiVector3t<float> aiPosition;

	aiMatrix.Decompose(aiScaling, aiRotation, aiPosition);

	Scene::Node node;
	node.id = scene->nodes.size();

	node.localPosition.x = aiPosition.x;
	node.localPosition.y = aiPosition.y;
	node.localPosition.z = aiPosition.z;

	node.localRotation.x = aiRotation.x;
	node.localRotation.y = aiRotation.y;
	node.localRotation.z = aiRotation.z;
	node.localRotation.w = aiRotation.w;

	node.localScale.x = aiScaling.x;
	node.localScale.y = aiScaling.y;
	node.localScale.z = aiScaling.z;


	scene->nodes.push_back(node);

	return node.id;
}
