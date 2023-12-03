#pragma once

#include <Core/CoreFwd.h>

class GameSceneSystem : public Mani::SystemBase
{
public:
	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;

protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;
	virtual void onDeinitialize(Mani::EntityRegistry& registry) override;
};