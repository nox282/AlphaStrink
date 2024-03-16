#pragma once

#include <Core/CoreFwd.h>

namespace Mani
{
	class SceneSystem;
}

class PlayAreaSystem;

struct LevelBlockComponent
{
	glm::vec3 frontAnchorOffset;
};

class LevelBlockSystem : public Mani::SystemBase
{
public:
	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;
	
	virtual void tick(float deltaTime, Mani::EntityRegistry& registry) override;

protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;
	virtual void onDeinitialize(Mani::EntityRegistry& registry) override;

private:
	std::weak_ptr<Mani::SceneSystem> m_sceneSystem;
	std::weak_ptr<PlayAreaSystem> m_playAreaSystem;

	std::vector<Mani::EntityId> m_levelBlocks;

	void spawnLevelBlock(Mani::EntityRegistry& registry);
	void moveFirstLevelBlockIfNecessary(Mani::EntityRegistry& registry);

	glm::vec3 getLastLevelBlockAnchor(const Mani::EntityRegistry& registry) const;
};