#pragma once

#include <Core/CoreFwd.h>

namespace Mani
{
	class InputSystem;
	class AssetSystem;
}

class PlayAreaSystem;

struct Reticule
{
	uint32_t playerId;
	glm::vec3 previousPosition;
};

namespace Actions
{
	const std::string AIM_ACTION = "AimPlayer";
}

class ReticuleSystem : public Mani::SystemBase
{
public:

	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;

	virtual void tick(float deltaTime, Mani::EntityRegistry& registry) override;

	Mani::EntityId spawnReticule(uint32_t playerId, Mani::EntityRegistry& registry);
protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;
	virtual void onDeinitialize(Mani::EntityRegistry& registry) override;

private:
	std::weak_ptr<Mani::InputSystem> m_inputSystem;
	std::weak_ptr<Mani::AssetSystem> m_assetSystem;
	std::weak_ptr<PlayAreaSystem> m_playAreaSystem;

	std::vector<Mani::EntityId> m_reticuleIds;
};