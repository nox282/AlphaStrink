#include "LevelBlockSystem.h"

#include "Game/Systems/PlayAreaSystem.h"

#include <Scene/SceneSystem.h>

using namespace Mani;

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
	if (m_levelBlocks.size() == 0)
	{
		spawnLevelBlock(registry);
		spawnLevelBlock(registry);
		spawnLevelBlock(registry);
		return;
	}

	EntityId lastLevelBlockId = m_levelBlocks.back();
	const Transform* lastLevelBlockTransform = registry.getComponent<Transform>(lastLevelBlockId);
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
		EntityId lastLevelBlockId = m_levelBlocks.back();
		const LevelBlockComponent* lastLevelBlock = registry.getComponent<LevelBlockComponent>(lastLevelBlockId);
		MANI_ASSERT(lastLevelBlock != nullptr, "level blocks cannot be created without a level block component.");
		const Transform* lastLevelBlockTransform = registry.getComponent<Transform>(lastLevelBlockId);
		MANI_ASSERT(lastLevelBlockTransform != nullptr, "level blocks cannot be created without a transform component.");

		spawnPoint = lastLevelBlockTransform->position + (playAreaTransform->forward() * 200.0f);
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
	//levelBlock->frontAnchorOffset = glm::vec3(-300.0f, 0.f, 325.f);

	m_levelBlocks.push_back(sceneEntityId);
}