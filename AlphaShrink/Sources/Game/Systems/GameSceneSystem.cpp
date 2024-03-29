#include "GameSceneSystem.h"

#include <Assets/AssetSystem.h>

#include <Scene/SceneSystem.h>

#include <RenderAPI/MeshComponent.h>
#include <RenderAPI/Mesh.h>
#include <RenderAPI/Material.h>
#include <RenderAPI/Light/DirectionalLightComponent.h>

std::string_view GameSceneSystem::getName() const
{
	return "GameSceneSystem";
}

bool GameSceneSystem::shouldTick(Mani::EntityRegistry& registry) const
{
	return false;
}

void GameSceneSystem::onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer)
{
	Mani::EntityId sunEntityId = registry.create();
	
	std::shared_ptr<Mani::AssetSystem> assetSystem = systemContainer.initializeDependency<Mani::AssetSystem>().lock();

	Mani::MeshComponent* meshComponent = registry.addComponent<Mani::MeshComponent>(sunEntityId);
	meshComponent->mesh = assetSystem->loadJsonAsset<Mani::Mesh>("AlphaShrink/Assets/Meshes/Sphere.mesh");
	meshComponent->material = assetSystem->loadJsonAsset<Mani::Material>("AlphaShrink/Assets/Materials/light.material");

	Mani::Transform* transform = registry.addComponent<Mani::Transform>(sunEntityId);
	transform->position = glm::vec3(5000.f, 1000.0f, 5000.0f);
	transform->scale = glm::vec3(50.0f, 50.0f, 50.0f);

	Mani::DirectionalLightComponent* light = registry.addComponent<Mani::DirectionalLightComponent>(sunEntityId);
	light->ambient = glm::vec3(.1f, .1f, .01f);
	light->diffuse = glm::vec3(.9f, .9f, .8f);
	light->specular = glm::vec3(.9f, .9f, .8f);
	light->direction = glm::normalize(-transform->position);
}

void GameSceneSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
}
