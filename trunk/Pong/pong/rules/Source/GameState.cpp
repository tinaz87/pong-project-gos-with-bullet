#include "GameState.h"
#include "physicsBody.h"
#include "physicsComponent.h"
#include "GameObjectSystem.h"
#include "ScoreManager.h"
#include "glog/logging.h"
#include <sstream>


const ObjectId GameState::GAME_STATE_TEXT_PLAYER_1 = "SCOREPLAYER_1";
const ObjectId GameState::GAME_STATE_TEXT_PLAYER_2 = "SCOREPLAYER_2";

GameState::GameState(const ObjectId& stateId)
	:FSMState(stateId)
	,m_ballBody(NULL),m_speed(100),m_publisherCollisionEvent(NULL),inGameScorePlayer1(NULL),m_publisherScoreEvent(NULL),inGameScorePlayer2(NULL)
	
{
	ObjectProperty* property= GameObjectSystem::GetSingleton().editProperty(PhysicsBody::PHY_BODY_ID, "ball1");
	if(property != NULL)
	{
		m_ballBody= static_cast<PhysicsBody*>(property);
	}

	m_subscriberCollisionEvent.SetSubscriber(this);
	m_subscriberScoreEvent.SetSubscriber(this);
	DLOG_ASSERT(m_ballBody);
}

void GameState::CollisionEvent(const CollisionData& data){

	// Check collision with lateral Wall
	if ( ObjectId("wall3") == data.getObjectIdA() || ObjectId("wall3") == data.getObjectIdB() ||
		ObjectId("wall4") == data.getObjectIdA() || ObjectId("wall4") == data.getObjectIdB())
	{
		btVector3 vel = m_ballBody->editBody()->getLinearVelocity();
		vel.normalize();
		
		float m_cos = vel.dot(btVector3(1.0f,0,0));

		if(	abs(m_cos) < 0.05f){

			float speed = m_ballBody->getSpeed();
			btVector3 factor;
			if( abs(m_ballBody->editBody()->getLinearVelocity().getX()) >= 0.0006f){
				
				factor =btVector3( m_ballBody->editBody()->getLinearVelocity().getX()/0.1f,m_ballBody->editBody()->getLinearVelocity().getY(),m_ballBody->editBody()->getLinearVelocity().getZ());

				DLOG(INFO)<<"IF: "<<abs(m_cos)<<" fact: "<<factor.getX()<<"x: "<<m_ballBody->editBody()->getLinearVelocity().getX();
				
			}
			else{

				factor = (m_ballBody->editBody()->getLinearVelocity() + btVector3(m_ballBody->editBody()->getLinearVelocity().getX() + ( 20 - rand() %41 ),0,0)) ;

				DLOG(INFO)<<"ELSE: "<<abs(m_cos)<<" fact: "<<factor.getX();
			}

			factor.normalize();

			factor*=speed;

			m_ballBody->editBody()->setLinearVelocity(factor);
			

		}

	}

	


}

void GameState::ScoreEvent(const ScoreData& data){

	std::stringstream sstring;

	sstring<< "Points: "<< data.getScoreA();

	inGameScorePlayer1->text = sstring.str();

	sstring.str("");

	sstring<< "Points: "<< data.getScoreB();

	inGameScorePlayer2->text = sstring.str();
}

void GameState::setCollisionPublisher(Publisher<CollisionObserver>* publisher){

	m_publisherCollisionEvent = publisher;
	m_subscriberCollisionEvent.Subscribe(publisher);
}

void GameState::setScorePublisher(Publisher<ScoreObserver>* publisher){

	m_publisherScoreEvent = publisher;
	m_subscriberScoreEvent.Subscribe(publisher);

}
void GameState::onEnter(){

	if (!m_publisherScoreEvent)	{
		
		setScorePublisher(&(ScoreManager::GetSingleton().getScorePublisher()));
	}

	if (!m_publisherCollisionEvent)
	{
		PhysicsComponent* phyCmp = static_cast<PhysicsComponent*>(GameObjectSystem::GetSingleton().editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID));

		if (phyCmp!= NULL)
		{
			
			setCollisionPublisher(&phyCmp->getCollisionPublisher());
		}
	}

	if (!inGameScorePlayer1 && !inGameScorePlayer2)
	{
		inGameScorePlayer1 = MV_NEW GfxInterfaceText();

		inGameScorePlayer1->rect = InterfaceRectangle(20,20,200,200);


		inGameScorePlayer2 = MV_NEW GfxInterfaceText();

		inGameScorePlayer2->rect = InterfaceRectangle(700,20,200,200);


		ObjectProperty* prop = GameObjectSystem::GetSingleton().editProperty(GfxInterface::INTERFACE_PROPERTY_ID,GfxInterface::INTERFACE_PROPERTY_OBJ_ID);

		if (prop != NULL)
		{
			GfxInterface* gfxInterface = static_cast<GfxInterface*>(prop);

			gfxInterface->addText(GAME_STATE_TEXT_PLAYER_1,inGameScorePlayer1);

			gfxInterface->addText(GAME_STATE_TEXT_PLAYER_2,inGameScorePlayer2);
		}


	}

	std::stringstream sstring;

	sstring<< "Points:"<< ScoreManager::GetSingleton().getScore(1);

	inGameScorePlayer1->text = sstring.str();

	sstring.str("");

	sstring<< "Points:"<< ScoreManager::GetSingleton().getScore(2);

	inGameScorePlayer2->text = sstring.str();

	inGameScorePlayer1->active = true;
	inGameScorePlayer2->active = true;


}


void GameState::onLeave(){

	//inGameScorePlayer1->active = false;
	//inGameScorePlayer2->active = false;

}

void GameState::onFrame(real frametime, real timestep){	

	m_ballBody->setSpeed( m_ballBody->editBody()->getLinearVelocity().length() + frametime * 5.0f );

	if(m_ballBody->getSpeed() > m_ballBody->getMaximumSpeed())
	{
		m_ballBody->setSpeed(m_ballBody->getMaximumSpeed());
	}


	
}
