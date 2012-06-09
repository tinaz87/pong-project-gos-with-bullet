#pragma once
#include "FSMState.h"
#include "StringHash.h"
#include "gfxFont.h"

class GameObjectSystem;

class EndState: public FSMState
{
public:
	EndState(const ObjectId& stateId);
	~EndState(){}
	virtual void onEnter();
	virtual void onLeave();
	virtual void onFrame(real frametime, real timestep) {}
private:



	GfxFont*	m_main;
	GfxFont*	m_second;

};
