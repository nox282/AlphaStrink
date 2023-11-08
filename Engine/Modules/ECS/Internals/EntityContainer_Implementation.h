#pragma once

#include <ECS.h>
#include <internals/EntityContainer.h>
#include <vector>
#include "Entity.h"
#include <unordered_map>

namespace ECSEngine {
	class EntityContainer_Implementation : public IEntityContainer
	{

#define INITIAL_COMPONENT_COUNT 1000

	public:
		// IEntityContainer begin
		EntityId create() override;
		bool destroy(EntityId entityId) override;
		const Entity* getEntity(EntityId entityId) const override;
		size_t size() const override;
		bool isValid(EntityId entityId) const override;

		void* addComponent(EntityId entityId, ComponentId componentId, size_t componentSize) override;
		void* getComponent(EntityId entityId, ComponentId componentId) const override;
		ComponentId getComponentId(const std::type_index& typeIndex) override;
		bool removeComponent(EntityId entityId, ComponentId componentId) override;
		bool hasComponent(EntityId entityId, ComponentId componentId) const override;
		// IEntityContainer end

	private:
		struct ComponentPool {
			ComponentPool(size_t inElementsSize);

			inline void* Get(size_t index);
			
			std::vector<unsigned char> data;
			size_t capacity;
			size_t elementSize;
		};

		std::vector<ComponentPool*> m_componentPools;
		std::vector<Entity> m_entities;
		std::vector<EntityId> m_entityPool;
		std::unordered_map<std::type_index, ComponentId> m_componentIds;
	};
}