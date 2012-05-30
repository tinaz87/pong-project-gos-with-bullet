#include "PositionControllerProperty.h"


const StringHash PositionControllerProperty::POSITION_CONTROLLER_PROPERTY_ID= StringHash("posControllerProp");

PositionControllerProperty::PositionControllerProperty(const ObjectId& objectId, char up, char down)
	:ObjectProperty(POSITION_CONTROLLER_PROPERTY_ID, objectId)
	,m_up(up)
	,m_down(down)
{

}

PositionControllerProperty::~PositionControllerProperty()
{

}

char PositionControllerProperty::getUp() const 
{
	return m_up;
}

char PositionControllerProperty::getDown() const
{
	return m_down;
}