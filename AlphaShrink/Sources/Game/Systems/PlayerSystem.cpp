#include "PlayerSystem.h"

#include <Core/Components/Transform.h>

#include <Camera/CameraSystem.h>

#include <OpenGL/OpenGLSystem.h>

#include <Inputs/InputSystem.h>

#include <RenderAPI/MeshComponent.h>
#include <RenderAPI/Mesh.h>
#include <RenderAPI/Material.h>

#include <Assets/AssetSystem.h>

#include <Game/Components/BoxComponent.h>

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
		inputSystem->addAction(Mani::LOCAL_USERID, AIM_ACTION);

		inputSystem->addBinding(Mani::LOCAL_USERID, MOVE_ACTION, "WASD");
		inputSystem->addBinding(Mani::LOCAL_USERID, AIM_ACTION, "Mouse");
	}

	m_cameraSystem = systemContainer.initializeDependency<Mani::CameraSystem>();

	std::shared_ptr<Mani::AssetSystem> assetSystem = systemContainer.initializeDependency<Mani::AssetSystem>().lock();

	m_playerEntityId = registry.create();
	Mani::Transform* playerTransform = registry.addComponent<Mani::Transform>(m_playerEntityId);
	registry.addComponent<Player>(m_playerEntityId);
	
	Mani::MeshComponent* meshComponent = registry.addComponent<Mani::MeshComponent>(m_playerEntityId);
	meshComponent->mesh = assetSystem->loadJsonAsset<Mani::Mesh>("AlphaShrink/Assets/Meshes/Cylinder.004.mesh");
	meshComponent->material = assetSystem->loadJsonAsset<Mani::Material>("AlphaShrink/Assets/Materials/cactus.material");

	m_playAreaEntityId = registry.create();
	Mani::Transform* playAreaTransform = registry.addComponent<Mani::Transform>(m_playAreaEntityId);
	playAreaTransform->position = glm::vec3(0.f, 0.f, 5.f);
	playAreaTransform->rotation = glm::angleAxis(glm::radians(45.f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	BoxComponent* boxComponent = registry.addComponent<BoxComponent>(m_playAreaEntityId);
	boxComponent->extent = glm::vec3(10.f, 10.f, 5.f);
}

void PlayerSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
}

void PlayerSystem::
tick(float deltaTime, Mani::EntityRegistry& registry)
{
	if (m_inputSystem.expired() || m_cameraSystem.expired())
	{
		return;
	}

	std::shared_ptr<Mani::InputSystem> inputSystem = m_inputSystem.lock();
	const Mani::InputAction* moveAction = inputSystem->getAction(Mani::LOCAL_USERID, MOVE_ACTION);
	const Mani::InputAction* aimAction = inputSystem->getAction(Mani::LOCAL_USERID, AIM_ACTION);

	glm::vec3 input(moveAction->x, moveAction->y, moveAction->z);
	if (glm::length2(input) <= FLT_EPSILON)
	{
		return;
	}

	BoxComponent* boxComponent = registry.getComponent<BoxComponent>(m_playAreaEntityId);
	Mani::Transform* boxTransform = registry.getComponent<Mani::Transform>(m_playAreaEntityId);

	Mani::Transform* playerTransform = registry.getComponent<Mani::Transform>(m_playerEntityId);
	playerTransform->rotation = boxTransform->rotation;
	playerTransform->position += (playerTransform->right() * input.x +
								playerTransform->up() * input.y +
								playerTransform->forward() * input.z) *
								deltaTime * 5.0f;

	glm::vec3 min, max;
	BoxComponent::getBounds(*boxComponent, *boxTransform, min, max);
	playerTransform->position = glm::clamp(playerTransform->position, min, max);
}

Mani::EntityId PlayerSystem::getPlayerAreaEntityId() const
{
	return m_playAreaEntityId;
}
