#include "PlayerSystem.h"

#include <OpenGL/OpenGLSystem.h>

#include <Inputs/InputSystem.h>

#include <Game/Systems/Player/ShipSystem.h>


std::string_view PlayerSystem::getName() const
{
	return "PlayerSystem";
}

bool PlayerSystem::shouldTick(Mani::EntityRegistry& registry) const
{
	return false;
}

void PlayerSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
{
	// initialize inputs
	std::shared_ptr<Mani::OpenGLSystem> openGLSystem = Mani::Application::get().getSystemContainer().getSystem<Mani::OpenGLSystem>().lock();
	std::shared_ptr<Mani::InputSystem> inputSystem = systemContainer.initializeDependency<Mani::InputSystem>().lock();
	inputSystem->assignInputGenerator(Mani::LOCAL_USERID, openGLSystem->getInputGenerator());

	m_shipSystem = systemContainer.initializeDependency<ShipSystem>();

	spawnPlayer(Mani::LOCAL_USERID, registry);
}

void PlayerSystem::spawnPlayer(uint32_t playerId, Mani::EntityRegistry& registry)
{
	if (m_shipSystem.expired())
	{
		return;
	}

	std::shared_ptr<ShipSystem> shipSystem = m_shipSystem.lock();
	shipSystem->spawnShip(playerId, registry);
}
