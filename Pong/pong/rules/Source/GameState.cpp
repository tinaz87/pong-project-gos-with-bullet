#include "GameState.h"
#include "physicsBody.h"
#include "GameObjectSystem.h"
#include "glog/logging.h"
#include <iostream>


GameState::GameState(const ObjectId& stateId)
	:FSMState(stateId)
	,m_ballBody(NULL),m_speed(100)
	
{
	ObjectProperty* property= GameObjectSystem::GetSingleton().editProperty(PhysicsBody::PHY_BODY_ID, "ball1");
	if(property != NULL)
	{
		m_ballBody= static_cast<PhysicsBody*>(property);
	}

	m_subscriberCollisionEvent.SetSubscriber(this);
	DLOG_ASSERT(m_ballBody);
}

void GameState::CollisionEvent(const CollisionData& data){}

void GameState::setCollisionPublisher(Publisher<CollisionObserver>* publisher){

	m_subscriberCollisionEvent.Subscribe(publisher);
}

void GameState::onFrame(real frametime, real timestep)
{

	btScalar velocity = m_ballBody->editBody()->getLinearVelocity().length();

	m_ballBody->setSpeed(velocity + frametime * 10.0f );

	if(m_ballBody->getSpeed() > m_ballBody->getMaximumSpeed())
	{
		m_ballBody->setSpeed(m_ballBody->getMaximumSpeed());
	}

	btVector3 vel = m_ballBody->editBody()->getLinearVelocity();
	vel.normalize();

	float cs = vel.dot(btVector3(1,0,0));

	if(	abs(cs) < 0.2f){

		m_ballBody->editBody()->setLinearVelocity(m_ballBody->editBody()->getLinearVelocity() + btVector3(10,0,0));


	}
	
}
