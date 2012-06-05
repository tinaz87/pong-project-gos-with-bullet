#include "position.h"

const StringHash Position::POSITION_ID= StringHash("Position");

Position::Position(const ObjectId& objectId, vector3 gfxOffset)
	:ObjectProperty(POSITION_ID, objectId)
{
	m_motionState= MV_NEW_ALIGNED(sizeof(m_motionState)) MotionState(gfxOffset);
}

Position::~Position()
{
	MV_DELETE(m_motionState);
}

void Position::setStartPosition(const matrix& position){

	startPosition = position;

}

matrix Position::getStartPosition() const{
	return startPosition;
}
matrix Position::getPosition() const
{
	return m_motionState->getGfxWorldTransformation();
}

void Position::setPosition(const matrix& position)
{
	m_motionState->setGfxWorldTransformation(position);
}

MotionState* Position::editMotionState()
{
	return m_motionState;
}

const MotionState* Position::getMotionState() const
{
	return m_motionState;
}


