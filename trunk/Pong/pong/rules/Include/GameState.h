#pragma once
#include "FSMState.h"
#include "StringHash.h"
#include "CollisionObserver.h"
#include "ScoreObserver.h"


class PhysicsBody;
class GfxInterfaceText;

class GameState: public FSMState,public CollisionObserver,public ScoreObserver
{
public:
	GameState(const ObjectId& stateId);
	~GameState(){}
	virtual void onEnter();
	virtual void onLeave();
	virtual void onFrame(real frametime, real timestep);

	void ComboCheck(const CollisionData& data);

	virtual void CollisionEvent(const CollisionData& data);
	virtual void ScoreEvent(const ScoreData& data);

	void setCollisionPublisher(Publisher<CollisionObserver>* publisher);
	void setScorePublisher(Publisher<ScoreObserver>* publisher);

private:

	PhysicsBody*	 m_ballBody;
	real			 m_speed;

	float oldSpeed;
	bool powerShoot;

	// Collision Oberver
	SubscriberHelper<CollisionObserver> m_subscriberCollisionEvent;
	Publisher<CollisionObserver>* m_publisherCollisionEvent;

	// Score Oberver
	SubscriberHelper<ScoreObserver> m_subscriberScoreEvent;
	Publisher<ScoreObserver>* m_publisherScoreEvent;

	static const ObjectId GAME_STATE_TEXT_PLAYER_1;
	static const ObjectId GAME_STATE_TEXT_PLAYER_2;

	GfxInterfaceText* inGameScorePlayer1;
	GfxInterfaceText* inGameScorePlayer2;
};
