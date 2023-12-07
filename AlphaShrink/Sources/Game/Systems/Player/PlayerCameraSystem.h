#pragma once

#include <Core/CoreFwd.h>

namespace Mani
{
	class CameraSystem;
}

class PlayerCameraSystem : public Mani::SystemBase
{
public:
	virtual std::string_view getName() const override;
	virtual bool shouldTick(Mani::EntityRegistry& registry) const override;
	virtual void tick(float deltaTime, Mani::EntityRegistry& registry) override;

protected:
	virtual void onInitialize(Mani::EntityRegistry& registry, Mani::SystemContainer& systemContainer) override;
	virtual void onDeinitialize(Mani::EntityRegistry& registry) override;

private:
	std::weak_ptr<Mani::CameraSystem> m_cameraSystem;
};