#include "ScoreManager.h"
#include "GameObjectSystem.h"
#include "position.h"

ScoreManager::ScoreManager(const std::string& ballId,
						   const std::string& bumperIdLeft,
						   const std::string& bumperIdRight):scoreLeftPlayer(0),scoreRightPlayer(0),objectID_BALL(ObjectId(ballId.c_str())),
						   objectID_LEFTWALL(ObjectId(bumperIdLeft.c_str())),objectID_RIGHTWALL(ObjectId(bumperIdRight.c_str()))
{	
	m_collisionSubscriber.SetSubscriber(this);
}


void ScoreManager::fireScore(const unsigned int iscoreA,const unsigned int iscoreB){
	
	ScoreData score(iscoreA,iscoreB);
	m_scorePublisher.RaiseEvent(&ScoreObserver::ScoreEvent,score);

}

void ScoreManager::setCollisionPublisher(PhysicsComponent* publisher){

	m_collisionSubscriber.Subscribe(&publisher->getCollisionPublisher());
}

Publisher<ScoreObserver>& ScoreManager::getScorePublisher(){

	return m_scorePublisher;
}

void ScoreManager::CollisionEvent(const CollisionData& data){

		checkCollision(data.getObjectIdA(),data.getObjectIdB());
}

void ScoreManager::checkCollision(const ObjectId& idCollisionA,const ObjectId& idCollisionB ){

	if ( idCollisionA == objectID_LEFTWALL || idCollisionB == objectID_LEFTWALL)
	{
		if (idCollisionA == objectID_BALL || idCollisionB == objectID_BALL)
		{

			DLOG(INFO) << "Collision Ball ( " << objectID_BALL.GetHash() <<" ) "<< " - LeftBumper ( " << objectID_LEFTWALL.GetHash()<<" )";
			++scoreLeftPlayer; 
			
		}

	}else{

		if ( idCollisionA == objectID_RIGHTWALL || idCollisionB == objectID_RIGHTWALL)
		{
			if (idCollisionA == objectID_BALL || idCollisionB == objectID_BALL)
			{

				DLOG(INFO) << "Collision Ball ( " << objectID_BALL.GetHash() <<" ) "<< " - RightBumper ( " << objectID_RIGHTWALL.GetHash()<<" )";
				++scoreRightPlayer;
			}
		}
	}

	fireScore(scoreLeftPlayer,scoreRightPlayer);

	return;


}


