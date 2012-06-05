#pragma once
#include "FSMState.h"
#include "StringHash.h"
#include "CollisionObserver.h"


class PhysicsBody;

class GameState: public FSMState,public CollisionObserver
{
public:
	GameState(const ObjectId& stateId);
	~GameState(){}
	virtual void onEnter() {}
	virtual void onLeave() {}
	virtual void onFrame(real frametime, real timestep);


	virtual void CollisionEvent(const CollisionData& data);


	void setCollisionPublisher(Publisher<CollisionObserver>* publisher);

private:

	PhysicsBody*	 m_ballBody;
	real			 m_speed;

	SubscriberHelper<CollisionObserver> m_subscriberCollisionEvent;
};
