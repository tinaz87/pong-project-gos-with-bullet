#pragma once
#include "Component.h"
#include "CollisionObserver.h"
#include "Subscriber.h"


class PositionControllerComponent: public Component,public CollisionObserver
{

public:
	static const StringHash POSITION_CONTROLLER_ID;
	PositionControllerComponent();
	virtual ~PositionControllerComponent();
	virtual void update(real frametime, real timestep);


	virtual void CollisionEvent(const CollisionData& data);


	void setCollisionPublisher(Publisher<CollisionObserver>* publisher);

private:

	SubscriberHelper<CollisionObserver> m_subscribeCollisionObserver;

	float oldSpeed;
	bool powerShoot;
};