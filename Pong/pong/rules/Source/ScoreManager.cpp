#include "ScoreManager.h"
#include "GameObjectSystem.h"
#include "position.h"
#include <glog/logging.h>
ScoreManager::ScoreManager(const ObjectId ball,const ObjectId leftWall
							,const ObjectId rightWall):scoreLeftPlayer(0),scoreRightPlayer(0)
{	
	
	objectID_BALL = ball;
	objectID_LEFTWALL = leftWall;
	objectID_RIGHTWALL = rightWall;
	m_collisionSubscriber.SetSubscriber(this);
}

ScoreManager::~ScoreManager(){}

void ScoreManager::CreateInstance(const ObjectId ball,const ObjectId leftWall,const ObjectId rightWall){

	if(GetSingletonPtr() == NULL)
		MV_NEW ScoreManager(ball,leftWall,rightWall);
}

void ScoreManager::reset(){

	scoreLeftPlayer = 0;
	scoreRightPlayer = 0;
}

void ScoreManager::fireScore(const unsigned int iscoreA,const unsigned int iscoreB){
	
	ScoreData score(iscoreA,iscoreB);
	m_scorePublisher.RaiseEvent(&ScoreObserver::ScoreEvent,score);

}

void ScoreManager::setCollisionPublisher(Publisher<CollisionObserver>* publisher){

	m_collisionSubscriber.Subscribe(publisher);
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

			DLOG(INFO) << "Collision Ball ( " << objectID_BALL.GetHash() <<" ) "<< " - LeftWall ( " << objectID_LEFTWALL.GetHash()<<" )";
			++scoreRightPlayer;

			fireScore(scoreLeftPlayer,scoreRightPlayer);
			
		}

	}else{

		if ( idCollisionA == objectID_RIGHTWALL || idCollisionB == objectID_RIGHTWALL)
		{
			if (idCollisionA == objectID_BALL || idCollisionB == objectID_BALL)
			{

				DLOG(INFO) << "Collision Ball ( " << objectID_BALL.GetHash() <<" ) "<< " - RightWall ( " << objectID_RIGHTWALL.GetHash()<<" )";
				++scoreLeftPlayer;

				fireScore(scoreLeftPlayer,scoreRightPlayer);
			}
		}
	}

	return;


}


