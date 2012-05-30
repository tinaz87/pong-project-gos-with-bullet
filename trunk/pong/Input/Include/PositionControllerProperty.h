#pragma once
#include "ObjectProperty.h"
#include "ObjectId.h"

class PositionControllerProperty: public ObjectProperty
{
public:
	static const StringHash POSITION_CONTROLLER_PROPERTY_ID;

	PositionControllerProperty(const ObjectId& objectId, char up, char down);
	~PositionControllerProperty();
	char getUp() const;
	char getDown() const;

private:
	char m_up;
	char m_down;
};