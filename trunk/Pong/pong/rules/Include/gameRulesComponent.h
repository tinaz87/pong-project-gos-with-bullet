#pragma once
#include "Component.h"
#include "Subscriber.h"
#include "FiniteStateMachine.h"
#include "ScoreObserver.h"

class ScoreManager;

class GameRulesComponent: public Component, public FiniteStateMachine, public ScoreObserver
{
public:
	static const StringHash RULES_COMPONENT_ID;
	GameRulesComponent();
	~GameRulesComponent();
	virtual void update(real frametime, real timestep);

	virtual void ScoreEvent(const ScoreData& score);

	void setScorePublisher(Publisher<ScoreObserver>* publisher);

private:


	static const ObjectId START_STATE;
	static const ObjectId GAME_STATE;
	static const ObjectId END_STATE;
	static const ObjectId PAUSE_STATE;
	static const ObjectId GOAL_SCORED;

	static const ObjectId KEY_START_PRESSED;
	static const ObjectId KEY_PAUSE_RELEASED;
	static const ObjectId KEY_RESTART;
	static const ObjectId SCORE_REACHED;
	

	SubscriberHelper<ScoreObserver> m_subscribeScoreObserver;

	bool m_keyPause;

	void configureFSM();
};
