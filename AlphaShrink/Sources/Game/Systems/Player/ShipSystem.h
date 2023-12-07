#pragma once

#include <Core/CoreFwd.h>

namespace Mani
{
	class InputSystem;
	class AssetSystem;
}

struct Ship
{
	uint32_t playerId;
	Mani::EntityId reticuleId;
};

class ReticuleSystem;

class ShipSystem : public Mani::SystemBase
{
public:
	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;

	virtual void tick(float deltaTime, Mani::EntityRegistry& registry) override;

	Mani::EntityId spawnShip(uint32_t playerId, Mani::EntityRegistry& registry);

protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;

private:
	std::weak_ptr<ReticuleSystem> m_reticuleSystem;
	std::weak_ptr<Mani::AssetSystem> m_assetSystem;
};