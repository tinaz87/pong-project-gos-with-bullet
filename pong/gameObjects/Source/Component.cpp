#include "Component.h"

Component::Component(const StringHash& componentId, uint8 updatePhase)
	:m_updatePhase(updatePhase)
	,m_componentId(componentId),m_active(true)
{

}

Component::~Component() 
{

}

uint8 Component::getUpdatePhase() const
{
	return m_updatePhase;
}

const StringHash& Component::getComponentId() const
{
	return m_componentId;
}

bool Component::IsActive() const{

	return m_active;
}

void Component::SetActiveStatus(const bool active){

	m_active = active;
}