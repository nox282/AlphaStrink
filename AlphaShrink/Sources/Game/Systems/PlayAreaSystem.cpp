#include "PlayAreaSystem.h"
#include <ECS/RegistryView.h>
#include "Game/Components/BoxComponent.h"

using namespace Mani;

std::string_view PlayAreaSystem::getName() const
{
	return "PlayAreaSystem";
}

bool PlayAreaSystem::shouldTick(EntityRegistry& registry) const
{
	return true;
}

void PlayAreaSystem::onInitialize(EntityRegistry& registry, SystemContainer& systemContainer)
{
	m_playAreaEntityId = registry.create();

	registry.addComponent<PlayArea>(m_playAreaEntityId);

	Transform* playAreaTransform = registry.addComponent<Transform>(m_playAreaEntityId);
	playAreaTransform->position = glm::vec3(0.f, 0.f, 5.f);
	playAreaTransform->rotation = glm::angleAxis(glm::radians(45.f), glm::vec3(0.0f, 1.0f, 0.0f));

	BoxComponent* boxComponent = registry.addComponent<BoxComponent>(m_playAreaEntityId);
	boxComponent->extent = glm::vec3(35.f, 20.f, 30.f);
}

void PlayAreaSystem::onDeinitialize(EntityRegistry& registry)
{
	registry.destroy(m_playAreaEntityId);
}

void PlayAreaSystem::tick(float deltaTime, EntityRegistry& registry)
{
	Transform* playAreaTransform = registry.getComponent<Transform>(m_playAreaEntityId);
	if (playAreaTransform == nullptr)
	{
		return;
	}
	
	playAreaTransform->position += playAreaTransform->forward() * 1.0f;

	const BoxComponent* playAreaBox = registry.getComponent<BoxComponent>(m_playAreaEntityId);
	RegistryView<Transform, PlayAreaEntity> view(registry);
	for (const auto& entityId : view)
	{
		Transform* transform = registry.getComponent<Transform>(entityId);

		// clamp local position
		transform->localPosition = glm::clamp(transform->localPosition, -playAreaBox->extent, playAreaBox->extent);
	}
}

const Transform* PlayAreaSystem::getPlayAreaTransform(const EntityRegistry& registry) const
{
	return registry.getComponent<Transform>(m_playAreaEntityId);
}

const BoxComponent* PlayAreaSystem::getPlayAreaBox(const EntityRegistry& registry) const
{
	return registry.getComponent<BoxComponent>(m_playAreaEntityId);
}

EntityId PlayAreaSystem::getPlayAreaEntityId() const
{
	return m_playAreaEntityId;
}
