#include "GameObjectSystem.h"
#include "ObjectProperty.h"
#include "Component.h"
#include "CustomMemory.h"

void GameObjectSystem::CreateInstance(uint16 maxObjectsCount, uint8 maxPhaseCount)
{
	if(GetSingletonPtr() == NULL)
		MV_NEW GameObjectSystem(maxObjectsCount, maxPhaseCount);
}

GameObjectSystem::GameObjectSystem(uint16 maxObjectCount, uint8 maxPhaseCount)
	:m_maxObjectsCount(maxObjectCount)
	,m_maxPhaseCount(maxPhaseCount)
{
	m_perPhaseComponentArray.resize(m_maxPhaseCount);
}

uint32 GameObjectSystem::getKeyForMap(const StringHash& id) const
{
	return id.GetHash();
}

GameObjectSystem::~GameObjectSystem()
{
	for(uint8 i= 0; i < m_perPhaseComponentArray.size(); ++i)
	{
		ComponentTable& componentMap= m_perPhaseComponentArray[i];
		for(ComponentTable::iterator cIt= componentMap.begin(); cIt != componentMap.end(); ++cIt)
		{
			MV_DELETE(cIt->second);
		}
		componentMap.clear();
	}
	m_perPhaseComponentArray.clear();

	for(PropertyTable::iterator pIt= m_propertyTable.begin(); pIt != m_propertyTable.end(); ++pIt)
	{
		ObjectPropertyTable& objectPropertyMap= pIt->second;
		for(ObjectPropertyTable::iterator opIt= objectPropertyMap.begin(); opIt != objectPropertyMap.end(); ++opIt)
		{
			MV_DELETE(opIt->second);
		}
		objectPropertyMap.clear();
	}
	m_propertyTable.clear();
}

void GameObjectSystem::addProperty(ObjectProperty* property)
{
	assert(property);
	PropertyTable::iterator pIt= m_propertyTable.find(getKeyForMap(property->getPropertyId()));
	if(pIt != m_propertyTable.end())
	{
		ObjectPropertyTable& objectPropertyMap= pIt->second;
		objectPropertyMap[getKeyForMap(property->getObjectId())]= property;
	}
	else
	{
		ObjectPropertyTable objectPropertyMap;
		objectPropertyMap[getKeyForMap(property->getObjectId())]= property;
		m_propertyTable[getKeyForMap(property->getPropertyId())]= objectPropertyMap;
	}
}

ObjectPropertyTable* GameObjectSystem::editProperties(const StringHash& propertyId)
{
	PropertyTable::iterator pmIt= m_propertyTable.find(getKeyForMap(propertyId));
	if(pmIt != m_propertyTable.end())
	{
		return &pmIt->second;
	}
	return NULL;
}

const ObjectPropertyTable* GameObjectSystem::getProperties(const StringHash& propertyId) const
{
	PropertyTable::const_iterator pmIt= m_propertyTable.find(getKeyForMap(propertyId));
	if(pmIt != m_propertyTable.end())
	{
		return &pmIt->second;
	}
	return NULL;
}

ObjectProperty* GameObjectSystem::editProperty(const StringHash& propertyId, const ObjectId& objectId)
{
	ObjectPropertyTable* obt= editProperties(propertyId);
	if(obt!=NULL)
	{
		ObjectPropertyTable::iterator obtIt= obt->find(getKeyForMap(objectId));
		if(obtIt != obt->end())
		{
			return obtIt->second;
		}
	}
	return NULL;
}

const ObjectProperty* GameObjectSystem::getProperty(const StringHash& propertyId, const ObjectId& objectId) const
{
	const ObjectPropertyTable* obt= getProperties(propertyId);
	if(obt!=NULL)
	{
		ObjectPropertyTable::const_iterator obtIt= obt->find(getKeyForMap(objectId));
		if(obtIt != obt->end())
		{
			return obtIt->second;
		}
	}
	return NULL;
}

void GameObjectSystem::addComponent(Component* component)
{
	assert(component);
	assert(component->getUpdatePhase() < m_maxPhaseCount);
	ComponentTable& componentMap= m_perPhaseComponentArray[component->getUpdatePhase()];
	componentMap[getKeyForMap(component->getComponentId())]= component;
}

Component* GameObjectSystem::editComponent(const StringHash& componentId, uint8 phase)
{
	uint8 startIndex= 0;
	uint8 endIndex= m_maxPhaseCount;
	if(phase < m_maxPhaseCount)
	{
		startIndex= phase;
		endIndex= startIndex + 1; 
	}

	for(uint8 i= startIndex; i < endIndex; ++i)
	{
		ComponentTable& componentMap= m_perPhaseComponentArray[i];
		ComponentTable::iterator cmIt= componentMap.find(getKeyForMap(componentId));
		if(cmIt != componentMap.end())
			return cmIt->second;
	}
	return NULL;
}

const Component* GameObjectSystem::getComponent(const StringHash& componentId, uint8 phase) const
{
	uint8 startIndex= 0;
	uint8 endIndex= m_maxPhaseCount;
	if(phase < m_maxPhaseCount)
	{
		startIndex= phase;
		endIndex= startIndex + 1; 
	}

	for(uint8 i= startIndex; i < endIndex; ++i)
	{
		const ComponentTable& componentMap= m_perPhaseComponentArray[i];
		ComponentTable::const_iterator cmIt= componentMap.find(getKeyForMap(componentId));
		if(cmIt != componentMap.end())
			return cmIt->second;
	}
	return NULL;
}

void GameObjectSystem::update(real frametime, real timestep)
{
	for(uint8 i= 0; i< m_perPhaseComponentArray.size(); ++i)
	{
		ComponentTable& componentMap= m_perPhaseComponentArray[i];
		for(ComponentTable::iterator cmIt= componentMap.begin(); cmIt != componentMap.end(); ++cmIt)
		{
			if (cmIt->second->IsActive())
				cmIt->second->update(frametime, timestep);
		}
	}
}
