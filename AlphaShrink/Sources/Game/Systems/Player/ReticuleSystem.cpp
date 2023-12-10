#include "ReticuleSystem.h"

#include <Assets/AssetSystem.h>

#include <RenderAPI/MeshComponent.h>
#include <RenderAPI/Mesh.h>
#include <RenderAPI/Material.h>

#include <Inputs/InputSystem.h>

#include "Game/Components/BoxComponent.h"
#include "Game/Systems/PlayAreaSystem.h"

std::string_view ReticuleSystem::getName() const
{
	return "ReticuleSystem";
}

bool ReticuleSystem::shouldTick(Mani::EntityRegistry& registry) const
{
	return true;
}

void ReticuleSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
{
	m_inputSystem = systemContainer.initializeDependency<Mani::InputSystem>();
	if (!m_inputSystem.expired())
	{
		std::shared_ptr<Mani::InputSystem> inputSystem = m_inputSystem.lock();
		inputSystem->addAction(Mani::LOCAL_USERID, Actions::AIM_ACTION);
		inputSystem->addBinding(Mani::LOCAL_USERID, Actions::AIM_ACTION, "Mouse");
	}

	m_assetSystem = systemContainer.initializeDependency<Mani::AssetSystem>();
	m_playAreaSystem = systemContainer.initializeDependency<PlayAreaSystem>();
}

void ReticuleSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
	for (const auto& entityId : m_reticuleIds)
	{
		registry.destroy(entityId);
	}
}

void ReticuleSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
{
	if (m_inputSystem.expired())
	{
		return;
	}

	std::shared_ptr<Mani::InputSystem> inputSystem = m_inputSystem.lock();

	Mani::RegistryView<Reticule, Mani::Transform> reticuleView(registry);
	for (const auto& entityId : reticuleView)
	{
		Reticule* reticule = registry.getComponent<Reticule>(entityId);
		Mani::Transform* transform = registry.getComponent<Mani::Transform>(entityId);

		const Mani::InputAction* aimAction = inputSystem->getAction(reticule->playerId, Actions::AIM_ACTION);
		if (aimAction == nullptr)
		{
			continue;
		}

		glm::vec3 input(aimAction->x, aimAction->y, 0.0f);
		const glm::vec3 delta = reticule->previousPosition - input;
		if (glm::length2(delta) <= FLT_EPSILON)
		{
			return;
		}

		transform->localPosition += delta * 0.1f;

		// we should have the option to consider mous inputs as normal axis
		reticule->previousPosition = input;
	}
}

Mani::EntityId ReticuleSystem::spawnReticule(uint32_t playerId, Mani::EntityRegistry& registry)
{
	if (m_assetSystem.expired() || m_playAreaSystem.expired())
	{
		return Mani::INVALID_ID;
	}

	std::shared_ptr<Mani::AssetSystem> assetSystem = m_assetSystem.lock();
	std::shared_ptr<PlayAreaSystem> playAreaSystem = m_playAreaSystem.lock();

	const BoxComponent* playArea = playAreaSystem->getPlayAreaBox(registry);
	if (playArea == nullptr)
	{
		return Mani::INVALID_ID;
	}

	Mani::EntityId entityId = registry.create();

	Mani::Transform* transform = registry.addComponent<Mani::Transform>(entityId);
	transform->parentId = playAreaSystem->getPlayAreaEntityId();
	transform->localPosition.z = playArea->extent.z;

	Reticule* reticule = registry.addComponent<Reticule>(entityId);
	reticule->playerId = playerId;

	Mani::MeshComponent* meshComponent = registry.addComponent<Mani::MeshComponent>(entityId);
	meshComponent->mesh = assetSystem->loadJsonAsset<Mani::Mesh>("AlphaShrink/Assets/Meshes/sphere.mesh");
	meshComponent->material = assetSystem->loadJsonAsset<Mani::Material>("AlphaShrink/Assets/Materials/green.material");

	m_reticuleIds.push_back(entityId);

	return entityId;
}
