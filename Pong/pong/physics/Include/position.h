#pragma once
#include "ObjectProperty.h"
#include "motionState.h"

class Position: public ObjectProperty
{
public:
	static const StringHash POSITION_ID;
	Position(const ObjectId& objectId, vector3 gfxOffset= vector3(0.f, 0.f, 0.f));
	~Position();

	matrix				getPosition() const;
	void				setPosition(const matrix& position);
	MotionState*		editMotionState();
	const MotionState*	getMotionState() const;

	matrix				getStartPosition() const;

	void				setStartPosition(const matrix& position);


private:
	MotionState*	m_motionState;
	matrix startPosition;
};