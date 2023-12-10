#pragma once

#include <Core/CoreFwd.h>

namespace Mani
{
	class InputSystem;
	class FloatingCameraControllerSystem;
	struct InputAction;
	struct EventHandle;
}

class PlayerCameraSystem;
class ReticuleSystem;
class ShipSystem;

class DebugSystem : public Mani::SystemBase
{
#if MANI_DEBUG
public:
	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;
protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;
	virtual void onDeinitialize(Mani::EntityRegistry& registry) override;
	
private:
	void onInputAction(uint32_t userId, const Mani::InputAction& inputAction);

	std::weak_ptr<Mani::InputSystem> m_inputSystem;
	std::weak_ptr<Mani::FloatingCameraControllerSystem> m_floatingCameraSystem;
	std::weak_ptr<PlayerCameraSystem> m_playerCameraSystem;
	std::weak_ptr<ReticuleSystem> m_reticuleSystem;

	Mani::EventHandle onInputActionHandle;
	bool isDebugging = false;

	const std::string TOGGLE_DEBUG_ACTION = "ToggleDebug";
#endif
};