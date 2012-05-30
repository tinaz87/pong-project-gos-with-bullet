#include "GameState.h"
#include "physicsBody.h"
#include "GameObjectSystem.h"
#include "glog/logging.h"

GameState::GameState(const ObjectId& stateId)
	:FSMState(stateId)
	,m_ballBody(NULL)
	,m_speed(100.f)
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
	m_ballBody->setSpeed(m_speed);
}