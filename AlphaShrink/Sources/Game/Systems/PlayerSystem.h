#pragma once

#include <Core/CoreFwd.h>

namespace Mani
{
	class InputSystem;
	class CameraSystem;
}

struct Player {};

class PlayerSystem : public Mani::SystemBase
{
public:
	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;

	virtual void tick(float deltaTime, Mani::EntityRegistry& registry) override;

	Mani::EntityId getPlayerAreaEntityId() const;

protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;
	virtual void onDeinitialize(Mani::EntityRegistry& registry) override;

private:
	std::weak_ptr<Mani::InputSystem> m_inputSystem;
	std::weak_ptr<Mani::CameraSystem> m_cameraSystem;
	
	Mani::EntityId m_playerEntityId;
	Mani::EntityId m_playAreaEntityId;

	const std::string MOVE_ACTION = "MovePlayer";
	const std::string AIM_ACTION = "AimPlayer";
};