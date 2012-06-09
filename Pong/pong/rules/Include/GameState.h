#pragma once
#include "FSMState.h"
#include "StringHash.h"
#include "CollisionObserver.h"
#include "ScoreObserver.h"
#include "gfxFont.h"

class GameObjectSystem;

class PhysicsBody;

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
	GfxFont*	m_left;
	GfxFont*	m_right;

	PhysicsBody*	 m_ballBody;
	real			 m_speed;

	float oldSpeed;
	bool powerShoot;

	// Collision Oberver
	SubscriberHelper<CollisionObserver> m_subscriberCollisionEvent;
	Publisher<CollisionObserver>* m_publisherCollisionEvent;

	void SetMessageStatusActive(const bool status);

	// Score Oberver
	SubscriberHelper<ScoreObserver> m_subscriberScoreEvent;
	Publisher<ScoreObserver>* m_publisherScoreEvent;

};
