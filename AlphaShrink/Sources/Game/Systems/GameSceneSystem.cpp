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
	transform->position = glm::vec3(500.f, 100.0f, 500.0f);
	transform->scale = glm::vec3(50.0f, 50.0f, 50.0f);

	Mani::DirectionalLightComponent* light = registry.addComponent<Mani::DirectionalLightComponent>(sunEntityId);
	light->ambient = glm::vec3(.1f, .1f, .01f);
	light->diffuse = glm::vec3(.9f, .9f, .8f);
	light->specular = glm::vec3(.9f, .9f, .8f);
	light->direction = glm::normalize(-transform->position);

	std::shared_ptr<Mani::SceneSystem> sceneSystem = systemContainer.initializeDependency<Mani::SceneSystem>().lock();
	Mani::EntityId sceneEntityId = sceneSystem->spawnScene(registry, "AlphaShrink/Assets/Meshes/U_ground_scene/U_ground_scene.scene");
	Mani::Transform* sceneTransform = registry.getComponent<Mani::Transform>(sceneEntityId);
	sceneTransform->position = glm::vec3(50.f, -20.f, 50.f);
	sceneTransform->rotation = glm::angleAxis(glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
	sceneTransform->scale = glm::vec3(2.f);
}

void GameSceneSystem::onDeinitialize(Mani::EntityRegistry& registry)
{
}
