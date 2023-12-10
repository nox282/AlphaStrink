#pragma once

#include <Core/CoreFwd.h>

namespace Mani
{
	struct Transform;
}

struct BoxComponent;

struct PlayArea {};
struct PlayAreaEntity {};

class PlayAreaSystem : public Mani::SystemBase
{
public:
	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;

	virtual void tick(float deltaTime, Mani::EntityRegistry& registry) override;

	const Mani::Transform* getPlayAreaTransform(const Mani::EntityRegistry& registry) const;
	const BoxComponent* getPlayAreaBox(const Mani::EntityRegistry& registry) const;
	Mani::EntityId getPlayAreaEntityId() const;

protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;
	virtual void onDeinitialize(Mani::EntityRegistry& registry) override;

private:
	Mani::EntityId m_playAreaEntityId;
};