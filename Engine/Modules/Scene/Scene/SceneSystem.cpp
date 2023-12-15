#include "SceneSystem.h"

#include <RenderAPI/MeshComponent.h>
#include <Assets/AssetSystem.h>

#include <unordered_map>

using namespace Mani;

std::string_view SceneSystem::getName() const
{
	return "SceneSystem";
}

bool SceneSystem::shouldTick(EntityRegistry& registry) const
{
	return false;
}

void SceneSystem::onInitialize(EntityRegistry& registry, SystemContainer& systemContainer)
{
	m_assetSystem = systemContainer.initializeDependency<AssetSystem>();
}

EntityId SceneSystem::spawnScene(EntityRegistry& registry, const std::filesystem::path& path)
{
	if (m_assetSystem.expired())
	{
		return INVALID_ID;
	}

	std::shared_ptr<AssetSystem> assetSystem = m_assetSystem.lock();
	std::shared_ptr<Scene> scene = assetSystem->loadJsonAsset<Scene>(path);
	if (scene == nullptr)
	{
		return INVALID_ID;
	}

	std::unordered_map<size_t, EntityId> nodeIdToEntityMap;
	for (const auto& node : scene->nodes)
	{
		nodeIdToEntityMap[node.id] = spawnNode(registry, node, assetSystem);
	}

	for (const auto& node : scene->nodes)
	{
		if (!node.hasParent())
		{
			continue;
		}

		const EntityId nodeEntityId = nodeIdToEntityMap[node.id];
		Transform* nodeTransform = registry.getComponent<Transform>(nodeEntityId);
		nodeTransform->parentId = nodeIdToEntityMap[node.parent];
	}

	return nodeIdToEntityMap[scene->root];
}

EntityId SceneSystem::spawnNode(EntityRegistry& registry, const Scene::Node& node, const std::shared_ptr<AssetSystem>& assetSystem)
{
	if (assetSystem == nullptr)
	{
		return INVALID_ID;
	}

	EntityId entityId = registry.create();

	Transform* transform = registry.addComponent<Transform>(entityId);
	transform->localPosition = node.localPosition;
	transform->localRotation = node.localRotation;
	transform->localScale = node.localScale;

	if (!node.meshAsset.empty())
	{
		std::shared_ptr<Mesh> mesh = assetSystem->loadJsonAsset<Mesh>(node.meshAsset);
		if (mesh == nullptr)
		{
			return INVALID_ID;
		}

		std::shared_ptr<Material> material = assetSystem->loadJsonAsset<Material>("AlphaShrink/Assets/Materials/floor.material");
		if (mesh == nullptr)
		{
			return INVALID_ID;
		}

		MeshComponent* meshComponent = registry.addComponent<MeshComponent>(entityId);
		meshComponent->mesh = mesh;
		meshComponent->material = material;
	}

	return entityId;
}
