#pragma once
#include "Component.h"
#include "Subscriber.h"
#include "FiniteStateMachine.h"

class GameRulesComponent: public Component, public FiniteStateMachine
{
public:
	static const StringHash RULES_COMPONENT_ID;
	GameRulesComponent();
	~GameRulesComponent();
	virtual void update(real frametime, real timestep);

private:
	static const ObjectId START_STATE;
	static const ObjectId GAME_STATE;
	static const ObjectId END_STATE;
	static const ObjectId PAUSE_STATE;

	static const ObjectId KEY_START_PRESSED;
	static const ObjectId KEY_PAUSE_RELEASED;
	static const ObjectId KEY_RESTART;
	static const ObjectId SCORE_REACHED;
	bool m_keyPause;

	void configureFSM();
};
