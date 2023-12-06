#include "PlayAreaSystem.h"
#include <ECS/RegistryView.h>
#include "Game/Components/BoxComponent.h"

std::string_view PlayAreaSystem::getName() const
{
	return "PlayAreaSystem";
}

bool PlayAreaSystem::shouldTick(Mani::EntityRegistry& registry) const
{
	return true;
}

void PlayAreaSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
{
	m_playAreaEntityId = registry.create();

	registry.addComponent<PlayArea>(m_playAreaEntityId);

	Mani::Transform* playAreaTransform = registry.addComponent<Mani::Transform>(m_playAreaEntityId);
	playAreaTransform->position = glm::vec3(0.f, 0.f, 5.f);
	playAreaTransform->rotation = glm::angleAxis(glm::radians(45.f), glm::vec3(0.0f, 1.0f, 0.0f));

	BoxComponent* boxComponent = registry.addComponent<BoxComponent>(m_playAreaEntityId);
	boxComponent->extent = glm::vec3(10.f, 10.f, 5.f);
}

void PlayAreaSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
	registry.destroy(m_playAreaEntityId);
}

void PlayAreaSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
{
	const Mani::Transform* playAreaTransform = registry.getComponent<Mani::Transform>(m_playAreaEntityId);
	const BoxComponent* playAreaBox = registry.getComponent<BoxComponent>(m_playAreaEntityId);

	Mani::RegistryView<Mani::Transform, PlayAreaChild> view(registry);
	for (const auto& entityId : view)
	{
		PlayAreaChild* playAreaChild = registry.getComponent<PlayAreaChild>(entityId);
		Mani::Transform* transform = registry.getComponent<Mani::Transform>(entityId);

		// clamp local position
		playAreaChild->localTransform.position = glm::clamp(playAreaChild->localTransform.position, -playAreaBox->extent, playAreaBox->extent);

		// apply world transformation
		transform->position = playAreaChild->localTransform.position;
		transform->rotation = playAreaChild->localTransform.rotation;
		transform->scale = playAreaChild->localTransform.scale;
		transform->apply(*playAreaTransform);
	}
}
