#include "ShipSystem.h"

#include <Assets/AssetSystem.h>

#include <RenderAPI/MeshComponent.h>
#include <RenderAPI/Mesh.h>
#include <RenderAPI/Material.h>

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
}

void ShipSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
{
	Mani::RegistryView<Ship, PlayAreaChild> shipView(registry);
	for (const auto& entityId : shipView)
	{
		Ship* ship = registry.getComponent<Ship>(entityId);
		PlayAreaChild* shipPlayAreaChild = registry.getComponent<PlayAreaChild>(entityId);

		PlayAreaChild* reticulePlayAreaChild = registry.getComponent<PlayAreaChild>(ship->reticuleId);
		if (reticulePlayAreaChild == nullptr)
		{
			continue;
		}

		glm::vec3 projectedTarget = glm::vec3(reticulePlayAreaChild->localTransform.position.x, reticulePlayAreaChild->localTransform.position.y, 0.f);
		glm::vec3 projectedPosition = glm::vec3(shipPlayAreaChild->localTransform.position.x, shipPlayAreaChild->localTransform.position.y, 0.f);

		
		glm::vec3 delta = projectedTarget - projectedPosition;
		const float length = glm::length(delta);
		if (length <= FLT_EPSILON)
		{
			continue;
		}
		const glm::vec3 direction = glm::normalize(reticulePlayAreaChild->localTransform.position - shipPlayAreaChild->localTransform.position);

		delta = glm::normalize(delta);
		shipPlayAreaChild->localTransform.position += delta * std::min(length, 50.f) * deltaTime;
		shipPlayAreaChild->localTransform.rotation = glm::quatLookAt(direction, glm::vec3(0.f, 1.f, 0.f));
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

	Mani::EntityId reticuleEntityId = reticuleSystem->spawnReticule(playerId, registry);
	if (reticuleEntityId == Mani::INVALID_ID)
	{
		return Mani::INVALID_ID;
	}

	Mani::EntityId entityId = registry.create();

	registry.addComponent<Mani::Transform>(entityId);

	Ship* ship = registry.addComponent<Ship>(entityId);
	ship->playerId = playerId;
	ship->reticuleId = reticuleEntityId;

	PlayAreaChild* playAreaChild = registry.addComponent<PlayAreaChild>(entityId);
	playAreaChild->localTransform.position.z = -10.f;

	Mani::MeshComponent* meshComponent = registry.addComponent<Mani::MeshComponent>(entityId);
	meshComponent->mesh = assetSystem->loadJsonAsset<Mani::Mesh>("AlphaShrink/Assets/Meshes/Cylinder.004.mesh");
	meshComponent->material = assetSystem->loadJsonAsset<Mani::Material>("AlphaShrink/Assets/Materials/cactus.material");

	return entityId;
}
