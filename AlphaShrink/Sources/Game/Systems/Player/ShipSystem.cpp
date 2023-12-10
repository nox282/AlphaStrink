#include "ShipSystem.h"

#include <Assets/AssetSystem.h>

#include <RenderAPI/MeshComponent.h>
#include <RenderAPI/Mesh.h>
#include <RenderAPI/Material.h>

#include "Game/Components/BoxComponent.h"
#include "Game/Systems/PlayAreaSystem.h"
#include "Game/Systems/Player/ReticuleSystem.h"

std::string_view ShipSystem::getName() const
{
	return "ShipSystem";
}

bool ShipSystem::shouldTick(Mani::EntityRegistry& registry) const
{
	return true;
}


void ShipSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
{
	m_reticuleSystem = systemContainer.initializeDependency<ReticuleSystem>();
	m_assetSystem = systemContainer.initializeDependency<Mani::AssetSystem>();
	m_playAreaSystem = systemContainer.initializeDependency<PlayAreaSystem>();
}

void ShipSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
	for (const auto& entityId : m_shipIds)
	{
		if (Ship* ship = registry.getComponent<Ship>(entityId))
		{
			registry.destroy(ship->reticuleId);
		}
		registry.destroy(entityId);
	}
}

void ShipSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
{
	Mani::RegistryView<Ship, Mani::Transform> shipView(registry);
	for (const auto& entityId : shipView)
	{
		Ship* ship = registry.getComponent<Ship>(entityId);
		Mani::Transform* shipTransform = registry.getComponent<Mani::Transform>(entityId);
		Mani::Transform* reticuleTransform = registry.getComponent<Mani::Transform>(ship->reticuleId);

		if (reticuleTransform == nullptr)
		{
			continue;
		}

		glm::vec3 projectedTarget = glm::vec3(reticuleTransform->localPosition.x, reticuleTransform->localPosition.y, 0.f);
		glm::vec3 projectedPosition = glm::vec3(shipTransform->localPosition.x, shipTransform->localPosition.y, 0.f);
		
		glm::vec3 delta = projectedTarget - projectedPosition;
		const float length = glm::length(delta);
		if (length <= FLT_EPSILON)
		{
			continue;
		}
		const glm::vec3 direction = glm::normalize(reticuleTransform->localPosition - shipTransform->localPosition);

		delta = glm::normalize(delta);
		shipTransform->localPosition += delta * std::min(length, 50.f) * deltaTime;
		shipTransform->localRotation = glm::quatLookAt(direction, glm::vec3(0.f, 1.f, 0.f));
	}
}

Mani::EntityId ShipSystem::spawnShip(uint32_t playerId, Mani::EntityRegistry& registry)
{
	if (m_reticuleSystem.expired() || m_assetSystem.expired())
	{
		return Mani::INVALID_ID;
	}

	std::shared_ptr<Mani::AssetSystem> assetSystem = m_assetSystem.lock();
	std::shared_ptr<ReticuleSystem> reticuleSystem = m_reticuleSystem.lock();
	std::shared_ptr<PlayAreaSystem> playAreaSystem = m_playAreaSystem.lock();

	const BoxComponent* playArea = playAreaSystem->getPlayAreaBox(registry);
	if (playArea == nullptr)
	{
		return Mani::INVALID_ID;
	}

	Mani::EntityId reticuleEntityId = reticuleSystem->spawnReticule(playerId, registry);
	if (reticuleEntityId == Mani::INVALID_ID)
	{
		return Mani::INVALID_ID;
	}

	Mani::EntityId entityId = registry.create();

	Mani::Transform* transform = registry.addComponent<Mani::Transform>(entityId);
	transform->parentId = playAreaSystem->getPlayAreaEntityId();
	transform->localPosition.z = -playArea->extent.z;

	Ship* ship = registry.addComponent<Ship>(entityId);
	ship->playerId = playerId;
	ship->reticuleId = reticuleEntityId;

	registry.addComponent<PlayAreaEntity>(entityId);

	Mani::EntityId shipModelId = registry.create();
	Mani::Transform* shipModelTransform = registry.addComponent<Mani::Transform>(shipModelId);
	shipModelTransform->parentId = entityId;
	shipModelTransform->localRotation = glm::angleAxis(glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));

	Mani::MeshComponent* meshComponent = registry.addComponent<Mani::MeshComponent>(shipModelId);
	meshComponent->mesh = assetSystem->loadJsonAsset<Mani::Mesh>("AlphaShrink/Assets/Meshes/Ship/ship.mesh");
	meshComponent->material = assetSystem->loadJsonAsset<Mani::Material>("AlphaShrink/Assets/Materials/redLit.material");

	m_shipIds.push_back(entityId);
	m_shipIds.push_back(shipModelId);

	return entityId;
}
