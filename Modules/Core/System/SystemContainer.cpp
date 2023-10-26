#include "SystemContainer.h"

using namespace ECSEngine;

void SystemContainer::initialize()
{
	if (m_bIsInitialized)
	{
		return;
	}

	for (auto& system : m_systems)
	{
		system->initialize(m_registry, *this);
	}

	m_bIsInitialized = true;
}

void SystemContainer::deinitialize()
{
	if (!m_bIsInitialized)
	{
		return;
	}

	for (auto& system : m_systems)
	{
		system->deinitialize(m_registry);
	}

	m_bIsInitialized = false;
}

void SystemContainer::tick(float deltaTime)
{
	if (!m_bIsInitialized)
	{
		return;
	}

	for (auto& system : m_systems)
	{
		if (system->shouldTick(m_registry))
		{
			system->tick(deltaTime, m_registry);
		}
	}
}
