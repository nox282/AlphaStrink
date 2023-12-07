#include "ReticuleSystem.h"

#include <Assets/AssetSystem.h>

#include <RenderAPI/MeshComponent.h>
#include <RenderAPI/Mesh.h>
#include <RenderAPI/Material.h>

#include <Inputs/InputSystem.h>

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
}

void ReticuleSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
}

void ReticuleSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
{
	if (m_inputSystem.expired())
	{
		return;
	}

	std::shared_ptr<Mani::InputSystem> inputSystem = m_inputSystem.lock();

	Mani::RegistryView<Reticule, PlayAreaChild> reticuleView(registry);
	for (const auto& entityId : reticuleView)
	{
		Reticule* reticule = registry.getComponent<Reticule>(entityId);
		PlayAreaChild* playAreaChild = registry.getComponent<PlayAreaChild>(entityId);

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

		playAreaChild->localTransform.position += delta * 0.1f;

		// we should have the option to consider mous inputs as normal axis
		reticule->previousPosition = input;
	}
}

Mani::EntityId ReticuleSystem::spawnReticule(uint32_t playerId, Mani::EntityRegistry& registry)
{
	if (m_assetSystem.expired())
	{
		return Mani::INVALID_ID;
	}

	std::shared_ptr<Mani::AssetSystem> assetSystem = m_assetSystem.lock();

	Mani::EntityId entityId = registry.create();

	registry.addComponent<Mani::Transform>(entityId);

	Reticule* reticule = registry.addComponent<Reticule>(entityId);
	reticule->playerId = playerId;

	PlayAreaChild* playAreaChild = registry.addComponent<PlayAreaChild>(entityId);
	playAreaChild->localTransform.position.z = 10.f;
	playAreaChild->isBoundToPlayArea = false;

	Mani::MeshComponent* meshComponent = registry.addComponent<Mani::MeshComponent>(entityId);
	meshComponent->mesh = assetSystem->loadJsonAsset<Mani::Mesh>("AlphaShrink/Assets/Meshes/sphere.mesh");
	meshComponent->material = assetSystem->loadJsonAsset<Mani::Material>("AlphaShrink/Assets/Materials/green.material");

	return entityId;
}
