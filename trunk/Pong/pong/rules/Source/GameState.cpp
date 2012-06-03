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
	DLOG_ASSERT(m_ballBody);
}

void GameState::onFrame(real frametime, real timestep)
{
	btScalar velocity = m_ballBody->editBody()->getLinearVelocity().length();
	btVector3 normVelocity = m_ballBody->editBody()->getLinearVelocity().normalized();
	normVelocity = normVelocity * (velocity + frametime*7.0f);
	m_ballBody->editBody()->setLinearVelocity( normVelocity ); 
	
}
