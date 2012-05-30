#pragma once
#include "ObjectId.h"

class ObjectProperty
{
public:
	ObjectProperty(const StringHash& propertyId, const ObjectId& objectId);
	virtual ~ObjectProperty();
	const StringHash& getPropertyId() const;
	const StringHash& getObjectId() const;

private:
	StringHash	m_propertyId;
	ObjectId	m_objectId;
};