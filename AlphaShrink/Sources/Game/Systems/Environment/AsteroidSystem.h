#pragma once

#include <Core/CoreFwd.h>

//namespace Mani
//{
//	class AssetSystem;
//}
//
//struct Asteroid
//{
//	glm::vec3 velocity;
//};
//
//class AsteroidSystem : public Mani::SystemBase
//{
//public:
//	virtual std::string_view getName() const override;
//	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;
//	virtual void tick(float deltaTime, Mani::EntityRegistry& registry) override;
//
//	Mani::EntityId spawnAsteroid(Mani::EntityRegistry& registry);
//
//protected:
//	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;
//	virtual void onDeinitialize(Mani::EntityRegistry& registry) override;
//
//private:
//	std::weak_ptr<Mani::AssetSystem> m_assetSystem;
//	std::weak_ptr<const PlayAreaSystem> m_playAreaSystem;
//	
//	std::vector<Mani::EntityId> m_asteroidIds;
//};