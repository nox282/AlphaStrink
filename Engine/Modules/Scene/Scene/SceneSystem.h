#pragma once

#include <Core/CoreFwd.h>
#include <Scene/Scene.h>
#include <filesystem>

namespace Mani
{
	class AssetSystem;

	class SceneSystem : public SystemBase
	{
	public:
		virtual std::string_view getName() const override;
		virtual bool shouldTick(EntityRegistry& registry) const override;

		EntityId spawnScene(EntityRegistry& registry, const std::filesystem::path& path);

	protected:
		virtual void onInitialize(EntityRegistry& registry, SystemContainer& systemContainer) override;
	
	private:
		std::weak_ptr<AssetSystem> m_assetSystem;

		EntityId spawnNode(EntityRegistry& registry, const Scene::Node& node, const std::shared_ptr<AssetSystem>& assetSystem);
	};
}