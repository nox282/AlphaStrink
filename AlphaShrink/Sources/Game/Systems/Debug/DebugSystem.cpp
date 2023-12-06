#include "DebugSystem.h"

#if MANI_DEBUG
#include <FloatingCamera/FloatingCameraControllerSystem.h>
#include <Inputs/InputSystem.h>

#include "Game/Systems/PlayerCameraSystem.h"
#include "Game/Systems/PlayerSystem.h"

std::string_view DebugSystem::getName() const
{
	return "DebugSystem";
}

bool DebugSystem::shouldTick(Mani::EntityRegistry& registry) const
{
	return false;
}

void DebugSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
{
	m_inputSystem = systemContainer.initializeDependency<Mani::InputSystem>();
	if (!m_inputSystem.expired())
	{
		std::shared_ptr<Mani::InputSystem> inputSystem = m_inputSystem.lock();
		inputSystem->addAction(Mani::LOCAL_USERID, TOGGLE_DEBUG_ACTION);
		inputSystem->addBinding(Mani::LOCAL_USERID, TOGGLE_DEBUG_ACTION, "F7");

		onInputActionHandle = inputSystem->onActionEvent.subscribe(std::bind(&DebugSystem::onInputAction, this, std::placeholders::_1, std::placeholders::_2));
	}

	m_floatingCameraSystem = systemContainer.initializeDependency<Mani::FloatingCameraControllerSystem>();
	m_playerCameraSystem = systemContainer.initializeDependency<PlayerCameraSystem>();
	m_playerSystem = systemContainer.initializeDependency<PlayerSystem>();
}

void DebugSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
	if (!m_inputSystem.expired())
	{
		std::shared_ptr<Mani::InputSystem> inputSystem = m_inputSystem.lock();
	
		inputSystem->onActionEvent.unsubscribe(onInputActionHandle);
	}
}

void DebugSystem::onInputAction(uint32_t userId, const Mani::InputAction& inputAction)
{
	if (inputAction.name != TOGGLE_DEBUG_ACTION || !inputAction.isPressed)
	{
		return;
	}

	if (m_playerCameraSystem.expired() || m_floatingCameraSystem.expired() || m_playerSystem.expired())
	{
		return;
	}

	isDebugging = !isDebugging;

	std::shared_ptr<Mani::FloatingCameraControllerSystem> floatingCameraSystem = m_floatingCameraSystem.lock();
	std::shared_ptr<PlayerCameraSystem> playerCameraSystem = m_playerCameraSystem.lock();
	std::shared_ptr<PlayerSystem> playerSystem = m_playerSystem.lock();
	if (isDebugging)
	{
		playerCameraSystem->disable();
		playerSystem->disable();
		floatingCameraSystem->enable();
	}
	else
	{
		floatingCameraSystem->disable();
		playerSystem->enable();
		playerCameraSystem->enable();
	}
}
#endif;