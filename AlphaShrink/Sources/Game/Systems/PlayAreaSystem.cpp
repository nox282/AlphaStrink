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
	boxComponent->extent = glm::vec3(35.f, 20.f, 30.f);
}

void PlayAreaSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
	registry.destroy(m_playAreaEntityId);
}

void PlayAreaSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
{
	const BoxComponent* playAreaBox = registry.getComponent<BoxComponent>(m_playAreaEntityId);

	Mani::RegistryView<Mani::Transform, PlayAreaEntity> view(registry);
	for (const auto& entityId : view)
	{
		Mani::Transform* transform = registry.getComponent<Mani::Transform>(entityId);

		// clamp local position
		transform->localPosition = glm::clamp(transform->localPosition, -playAreaBox->extent, playAreaBox->extent);
	}
}

const Mani::Transform* PlayAreaSystem::getPlayAreaTransform(const Mani::EntityRegistry& registry) const
{
	return registry.getComponent<Mani::Transform>(m_playAreaEntityId);
}

const BoxComponent* PlayAreaSystem::getPlayAreaBox(const Mani::EntityRegistry& registry) const
{
	return registry.getComponent<BoxComponent>(m_playAreaEntityId);
}

Mani::EntityId PlayAreaSystem::getPlayAreaEntityId() const
{
	return m_playAreaEntityId;
}
