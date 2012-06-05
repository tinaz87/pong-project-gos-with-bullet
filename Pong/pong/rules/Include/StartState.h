#pragma once
#include "FSMState.h"
#include "StringHash.h"

class GfxInterfaceText;

class StartState: public FSMState
{
public:
	StartState(const ObjectId& stateId);
	~StartState(){}
	virtual void onEnter();
	virtual void onLeave();
	virtual void onFrame(real frametime, real timestep) {}
private:

	static const ObjectId START_STATE_TEXT_ID;

	GfxInterfaceText* startText;
};
