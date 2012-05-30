#pragma once
#include "FSMState.h"
#include "StringHash.h"

class PauseState: public FSMState
{
public:
	PauseState(const ObjectId& stateId);
	~PauseState(){}
	virtual void onEnter();
	virtual void onLeave();
	virtual void onFrame(real frametime, real timestep) {}
private:
};
