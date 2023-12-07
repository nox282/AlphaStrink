#include "PlayerCameraSystem.h"

#include <Camera/CameraSystem.h>

#include <Game/Systems/PlayAreaSystem.h>
#include <Game/Components/BoxComponent.h>

std::string_view PlayerCameraSystem::getName() const
{
    return "PlayerCameraSystem";
}

bool PlayerCameraSystem::shouldTick(Mani::EntityRegistry& registry) const
{
    return true;
}

void PlayerCameraSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
{
    m_cameraSystem = systemContainer.initializeDependency<Mani::CameraSystem>();
}

void PlayerCameraSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
}

void PlayerCameraSystem::tick(float deltaTime, Mani::EntityRegistry& registry)
{
    if (m_cameraSystem.expired())
    {
        return;
    }

    std::shared_ptr<Mani::CameraSystem> cameraSystem = m_cameraSystem.lock();
    
    Mani::RegistryView<Mani::Transform, PlayArea, BoxComponent> playAreaView(registry);

    if (playAreaView.begin() == playAreaView.end())
    {
        MANI_LOG_ERROR(Mani::Log, "No play area found");
        return;
    }

    const Mani::EntityId playAreaEntityId = *playAreaView.begin();

    BoxComponent* playerAreaBox = registry.getComponent<BoxComponent>(playAreaEntityId);
    Mani::Transform* playerAreaTransform = registry.getComponent<Mani::Transform>(playAreaEntityId);
   
    if (playerAreaBox == nullptr || playerAreaTransform == nullptr)
    {
        MANI_LOG_ERROR(Mani::Log, "Could not get play area");
        return;
    }

    Mani::Transform* cameraTransform = cameraSystem->getCameraTransform(registry);
    const Mani::CameraComponent* cameraComponent = cameraSystem->getCameraComponent(registry);

    const glm::vec3 zOffset = glm::rotate(playerAreaTransform->rotation, glm::vec3(0.f, 0.f, -playerAreaBox->extent.z));
    const glm::vec3 cameraTarget = playerAreaTransform->position + zOffset;
    
    cameraTransform->position = cameraTarget;
    cameraTransform->rotation = playerAreaTransform->rotation;
    
    const Mani::CameraConfig& config = cameraComponent->config;

    const float targetFrustrumWidth = playerAreaBox->extent.x * 2;
    const float targetFrustrumHeight = targetFrustrumWidth / config.getAspectRatio();
    const float targetDistance = targetFrustrumHeight * .5f / glm::tan(glm::radians(config.fov * .5f));
    
    const glm::vec3 direction = glm::normalize(cameraTransform->position - playerAreaTransform->position);
    cameraTransform->position = cameraTarget + direction * targetDistance;
}
