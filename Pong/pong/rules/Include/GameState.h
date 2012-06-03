#pragma once
#include "FSMState.h"
#include "StringHash.h"

class PhysicsBody;

class GameState: public FSMState
{
public:
	GameState(const ObjectId& stateId);
	~GameState(){}
	virtual void onEnter() {}
	virtual void onLeave() {}
	virtual void onFrame(real frametime, real timestep);

private:

	PhysicsBody*	 m_ballBody;
	real			 m_speed;
};
