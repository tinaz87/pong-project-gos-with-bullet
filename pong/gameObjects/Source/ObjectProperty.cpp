#include "ObjectProperty.h"

ObjectProperty::ObjectProperty(const StringHash& propertyId, const ObjectId& objectId)
	:m_propertyId(propertyId)
	,m_objectId(objectId)
{
}

ObjectProperty::~ObjectProperty() 
{
	m_propertyId= ObjectId("");
}

const StringHash& ObjectProperty::getPropertyId() const
{
	return m_propertyId;
}

const StringHash& ObjectProperty::getObjectId() const
{
	return m_objectId;
}
