#include <Core/CoreFwd.h>

#include <Inputs/InputSystem.h>

#include <OpenGL/Render/OpenGLRenderSystem.h>
#include <OpenGL/OpenGLSystem.h>

#include "Game/Systems/Player/PlayerSystem.h"
#include "Game/Systems/Player/ReticuleSystem.h"
#include "Game/Systems/Player/ShipSystem.h"
#include "Game/Systems/Player/PlayerCameraSystem.h"

#include "Game/Systems/GameSceneSystem.h"
#include "Game/Systems/PlayAreaSystem.h"
#include <Game/Systems/Debug/DebugSystem.h>
#include <Game/Systems/Environment/LevelBlockSystem.h>

#include <FloatingCamera/FloatingCameraControllerSystem.h>

using namespace Mani;

void initializeGameWorld(const std::shared_ptr<World>& world);

int main(int argc, char** argv)
{
	Application app;
	SystemContainer& appSystemContainer= app.getSystemContainer();
	appSystemContainer.createSystem<OpenGLSystem>();
	std::shared_ptr<WorldSystem> worldSystem = appSystemContainer.initializeDependency<WorldSystem>().lock();;

	std::shared_ptr<World> world = worldSystem->createWorld();
	initializeGameWorld(world);
	app.run();

	return EXIT_SUCCESS;
}

void initializeGameWorld(const std::shared_ptr<World>& world)
{
	SystemContainer& systemContainer = world->getSystemContainer();
	systemContainer.createSystem<OpenGLRenderSystem>()
		.createSystem<DebugSystem>()
		.createSystem<PlayAreaSystem>()
		.createSystem<PlayerSystem>()
		.createSystem<ReticuleSystem>()
		.createSystem<ShipSystem>()
		.createSystem<PlayerCameraSystem>()
		.createSystem<GameSceneSystem>()
		.createSystem<LevelBlockSystem>();
}