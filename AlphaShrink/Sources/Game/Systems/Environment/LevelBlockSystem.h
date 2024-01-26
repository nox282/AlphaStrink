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
	virtual std::string_view getName() const;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const;
	
	virtual void tick(float deltaTime, Mani::EntityRegistry& registry);

protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer);
	virtual void onDeinitialize(Mani::EntityRegistry& registry);

private:
	std::weak_ptr<Mani::SceneSystem> m_sceneSystem;
	std::weak_ptr<PlayAreaSystem> m_playAreaSystem;

	std::vector<Mani::EntityId> m_levelBlocks;

	void spawnLevelBlock(Mani::EntityRegistry& registry);
};