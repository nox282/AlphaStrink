#pragma once

#include <Core/CoreFwd.h>

class ShipSystem;

class PlayerSystem : public Mani::SystemBase
{
public:
	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;

	void spawnPlayer(uint32_t playerId, Mani::EntityRegistry& registry);

protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;

private:
	std::weak_ptr<ShipSystem> m_shipSystem;
};