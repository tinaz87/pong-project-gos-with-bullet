#pragma once
#include "FSMState.h"
#include "StringHash.h"

class StartState: public FSMState
{
public:
	StartState(const ObjectId& stateId);
	~StartState(){}
	virtual void onEnter();
	virtual void onLeave();
	virtual void onFrame(real frametime, real timestep) {}
private:
};
