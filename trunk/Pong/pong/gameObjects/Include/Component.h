#pragma once
#include "StringHash.h"

class Component
{
public:
	Component(const StringHash& componentId, uint8 updatePhase);
	virtual ~Component();
	uint8 getUpdatePhase() const;
	const StringHash& getComponentId() const;
	virtual void update(real frametime, real timestep)= 0;
	bool IsActive() const;
	void SetActiveStatus(bool active);
private:
	uint8 m_updatePhase;
	StringHash m_componentId;
	bool m_active;
};