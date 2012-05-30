#pragma once
#include "dataTypes.h"
#include "StringHash.h"
#include "ObjectId.h"
#include "Singleton.h"
#include "Allocator.h" //std::vector std::map stdext::hash_map includes

class ObjectProperty;
class Component;
typedef MvHashMap<uint32, ObjectProperty*>	ObjectPropertyTable;	//objectId -> property
typedef MvHashMap<uint32, ObjectPropertyTable>	PropertyTable;	    //propertyId -> objectPropertyMap
typedef MvHashMap<uint32, Component*>		ComponentTable;			//componentId -> component

class GameObjectSystem: public Singleton<GameObjectSystem>
{
public:
	static void CreateInstance(uint16 maxObjectsCount, uint8 maxPhaseCount);
	~GameObjectSystem();
	//properties
	void					addProperty(ObjectProperty* property);
	ObjectPropertyTable*	editProperties(const StringHash& propertyId);
	const ObjectPropertyTable*getProperties(const StringHash& propertyId) const;
	ObjectProperty*			editProperty(const StringHash& propertyId, const ObjectId& objectId);
	const ObjectProperty*	getProperty(const StringHash& propertyId, const ObjectId& objectId) const;

	//component
	void					addComponent(Component* component);
	Component*				editComponent(const StringHash& componentId, uint8 phase= uint8MAX);
	const Component*		getComponent(const StringHash& componentId, uint8 phase= uint8MAX) const;

	void update(real frametime, real timestep);	

private:
	GameObjectSystem(uint16 maxObjectsCount, uint8 maxPhaseCount);
	uint32						getKeyForMap(const StringHash& id) const;
	uint16						m_maxObjectsCount;
	uint8						m_maxPhaseCount;
	PropertyTable				m_propertyTable;
	MvVector<ComponentTable>	m_perPhaseComponentArray;
};
