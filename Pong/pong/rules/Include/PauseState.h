#pragma once
#include "FSMState.h"
#include "StringHash.h"
#include "gfxFont.h"

class GameObjectSystem;

class PauseState: public FSMState
{
public:
	PauseState(const ObjectId& stateId);
	~PauseState(){}
	virtual void onEnter();
	virtual void onLeave();
	virtual void onFrame(real frametime, real timestep) {}


private:



	GfxFont*	m_main;
	GfxFont*	m_second;

};
