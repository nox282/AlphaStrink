#include "LevelBlockSystem.h"

#include "Game/Systems/PlayAreaSystem.h"

#include <Scene/SceneSystem.h>

using namespace Mani;

constexpr float distanceToMove = 200.f;
constexpr size_t amountToSpawn = 20;

std::string_view LevelBlockSystem::getName() const
{
	return "LevelBlockSystem";
}

bool LevelBlockSystem::shouldTick(EntityRegistry& registry) const
{
	return true;
}

void LevelBlockSystem::onInitialize(EntityRegistry& registry, SystemContainer& systemContainer)
{
	m_sceneSystem = systemContainer.initializeDependency<SceneSystem>();
	m_playAreaSystem = systemContainer.initializeDependency<PlayAreaSystem>();
	for (size_t i = 0; i < amountToSpawn; i++)
	{
		spawnLevelBlock(registry);
	}
}

void LevelBlockSystem::onDeinitialize(EntityRegistry& registry)
{
	for (const auto& entityId : m_levelBlocks)
	{
		registry.destroy(entityId);
	}
}

void LevelBlockSystem::tick(float deltaTime, EntityRegistry& registry)
{
	if (m_playAreaSystem.expired())
	{
		return;
	}

	moveFirstLevelBlockIfNecessary(registry);
}

void LevelBlockSystem::spawnLevelBlock(EntityRegistry& registry)
{
	if (m_sceneSystem.expired() || m_playAreaSystem.expired())
	{
		return;
	}

	std::shared_ptr<PlayAreaSystem> playAreaSystem = m_playAreaSystem.lock();
	const Transform* playAreaTransform = playAreaSystem->getPlayAreaTransform(registry);
	MANI_ASSERT(playAreaTransform != nullptr, "play area cannot not have a transform");

	glm::vec3 spawnPoint = glm::vec3(0.f);
	
	if (m_levelBlocks.size() == 0)
	{	
		spawnPoint = playAreaTransform->position + glm::vec3(50.f, -20.f, 50.f);
	}
	else
	{
		spawnPoint = getLastLevelBlockAnchor(registry);
	}

	std::shared_ptr<SceneSystem> sceneSystem = m_sceneSystem.lock();
	EntityId sceneEntityId = sceneSystem->spawnScene(
		registry,
		"AlphaShrink/Assets/Meshes/U_ground_scene/U_ground_scene.scene",
		"AlphaShrink/Assets/Materials/floor.material"
	);

	Transform* sceneTransform = registry.getComponent<Transform>(sceneEntityId);
	sceneTransform->position = spawnPoint;
	sceneTransform->rotation = glm::angleAxis(glm::radians(-45.f), glm::vec3(0.0f, 1.0f, 0.0f));
	sceneTransform->scale = glm::vec3(2.f);

	LevelBlockComponent* levelBlock = registry.addComponent<LevelBlockComponent>(sceneEntityId);

	m_levelBlocks.push_back(sceneEntityId);
}

void LevelBlockSystem::moveFirstLevelBlockIfNecessary(Mani::EntityRegistry& registry)
{
	if (m_levelBlocks.size() == 0)
	{
		return;
	}

	std::shared_ptr<PlayAreaSystem> playAreaSystem = m_playAreaSystem.lock();
	const Transform* playerTransform = playAreaSystem->getPlayAreaTransform(registry);
	if (playerTransform == nullptr)
	{
		return;
	}

	EntityId firstLevelBlockId = m_levelBlocks.front();
	Transform* firstLevelBlockTransform = registry.getComponent<Transform>(firstLevelBlockId);
	if (firstLevelBlockTransform == nullptr)
	{
		return;
	}

	if (glm::distance2(firstLevelBlockTransform->position, playerTransform->position) < distanceToMove * distanceToMove)
	{
		return;
	}

	glm::vec3 anchor = getLastLevelBlockAnchor(registry);

	m_levelBlocks.erase(m_levelBlocks.begin());
	firstLevelBlockTransform->position = anchor;
	m_levelBlocks.push_back(firstLevelBlockId);
}

glm::vec3 LevelBlockSystem::getLastLevelBlockAnchor(const Mani::EntityRegistry& registry) const
{
	if (m_levelBlocks.size() == 0 || m_playAreaSystem.expired())
	{
		return glm::vec3(0.f);
	}

	std::shared_ptr<PlayAreaSystem> playAreaSystem = m_playAreaSystem.lock();
	const Transform* playAreaTransform = playAreaSystem->getPlayAreaTransform(registry);
	if (playAreaTransform == nullptr)
	{
		return glm::vec3(0.f);
	}
	
	EntityId lastLevelBlockId = m_levelBlocks.back();
	const LevelBlockComponent* lastLevelBlock = registry.getComponent<LevelBlockComponent>(lastLevelBlockId);
	MANI_ASSERT(lastLevelBlock != nullptr, "level blocks cannot be created without a level block component.");
	const Transform* lastLevelBlockTransform = registry.getComponent<Transform>(lastLevelBlockId);
	MANI_ASSERT(lastLevelBlockTransform != nullptr, "level blocks cannot be created without a transform component.");

	return lastLevelBlockTransform->position + (playAreaTransform->forward() * 200.0f);
}
