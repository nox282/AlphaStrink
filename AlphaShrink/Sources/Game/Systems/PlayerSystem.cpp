#include "PlayerSystem.h"

#include <OpenGL/OpenGLSystem.h>

#include <Inputs/InputSystem.h>

#include <RenderAPI/MeshComponent.h>
#include <RenderAPI/Mesh.h>
#include <RenderAPI/Material.h>

#include <Assets/AssetSystem.h>

#include <Game/Systems/PlayAreaSystem.h>


std::string_view PlayerSystem::getName() const
{
	return "PlayerSystem";
}

bool PlayerSystem::shouldTick(Mani::EntityRegistry& registry) const
{
	return true;
}

void PlayerSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
{
	// initialize inputs
	std::shared_ptr<Mani::OpenGLSystem> openGLSystem = Mani::Application::get().getSystemContainer().getSystem<Mani::OpenGLSystem>().lock();
	m_inputSystem = systemContainer.initializeDependency<Mani::InputSystem>();

	if (!m_inputSystem.expired())
	{
		std::shared_ptr<Mani::InputSystem> inputSystem = m_inputSystem.lock();
		inputSystem->assignInputGenerator(Mani::LOCAL_USERID, openGLSystem->getInputGenerator());
		inputSystem->addAction(Mani::LOCAL_USERID, MOVE_ACTION);
		inputSystem->addBinding(Mani::LOCAL_USERID, MOVE_ACTION, "WASD");
	}

	std::shared_ptr<Mani::AssetSystem> assetSystem = systemContainer.initializeDependency<Mani::AssetSystem>().lock();

	m_playerEntityId = registry.create();
	
	Mani::Transform* playerTransform = registry.addComponent<Mani::Transform>(m_playerEntityId);
	
	Player* player = registry.addComponent<Player>(m_playerEntityId);
	player->playerId = Mani::LOCAL_USERID;

	registry.addComponent<PlayAreaChild>(m_playerEntityId);
	
	Mani::MeshComponent* meshComponent = registry.addComponent<Mani::MeshComponent>(m_playerEntityId);
	meshComponent->mesh = assetSystem->loadJsonAsset<Mani::Mesh>("AlphaShrink/Assets/Meshes/Cylinder.004.mesh");
	meshComponent->material = assetSystem->loadJsonAsset<Mani::Material>("AlphaShrink/Assets/Materials/cactus.material");
}

void PlayerSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
	registry.destroy(m_playerEntityId);
}

void PlayerSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
{
	if (m_inputSystem.expired())
	{
		return;
	}

	std::shared_ptr<Mani::InputSystem> inputSystem = m_inputSystem.lock();

	Mani::RegistryView<Player, PlayAreaChild> playerView(registry);
	for (const auto& entityId : playerView)
	{
		Player* player = registry.getComponent<Player>(entityId);
		PlayAreaChild* playAreaChild = registry.getComponent<PlayAreaChild>(entityId);
		
		const Mani::InputAction* moveAction = inputSystem->getAction(player->playerId, MOVE_ACTION);
		if (moveAction == nullptr)
		{
			continue;
		}

		glm::vec3 input(moveAction->x, moveAction->y, moveAction->z);
		if (glm::length2(input) <= FLT_EPSILON)
		{
			return;
		}
		
		playAreaChild->localTransform.position += input * deltaTime * 10.0f;
	}
}
