#pragma once
#include "FSMState.h"
#include "StringHash.h"

class EndState: public FSMState
{
public:
	EndState(const ObjectId& stateId);
	~EndState(){}
	virtual void onEnter();
	virtual void onLeave() {}
	virtual void onFrame(real frametime, real timestep) {}
private:
};
