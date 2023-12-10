#include "AsteroidSystem.h"

#include <Assets/AssetSystem.h>

//std::string_view AsteroidSystem::getName() const
//{
//	return "AsteroidSystem";
//}
//
//bool AsteroidSystem::shouldTick(Mani::EntityRegistry& registry) const
//{
//	return true;
//}
//
//void AsteroidSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
//{
//	m_assetSystem = systemContainer.initializeDependency<Mani::AssetSystem>();
//}
//
//void AsteroidSystem::onDeinitialize(Mani::EntityRegistry& registry)
//{
//	for (const auto& entityId : m_asteroidIds)
//	{
//		registry.destroy(entityId);
//	}
//}
//
//void AsteroidSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
//{
//
//}
//
//Mani::EntityId AsteroidSystem::spawnAsteroid(Mani::EntityRegistry& registry)
//{
//	if (m_assetSystem.expired())
//	{
//		return Mani::INVALID_ID;
//	}
//
//	Mani::EntityId entityId = registry.create();
//
//
//
//	return entityId();
//}
