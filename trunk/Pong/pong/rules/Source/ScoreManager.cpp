#include "ScoreManager.h"
#include "GameObjectSystem.h"
#include "position.h"

ScoreManager::ScoreManager(const ObjectId& iobjectID_Ball,
						   const ObjectId& iobjectID_Left,
						   const ObjectId& iobjectID_Right):objectID_BALL(iobjectID_Ball),
															objectID_LEFTWALL(iobjectID_Left),
															objectID_RIGHTWALL(iobjectID_Right)
{	
	m_collisionSubscriber.SetSubscriber(this);
}


void ScoreManager::fireScore(const unsigned int iscoreA,const unsigned int iscoreB){
	
	ScoreData score(iscoreA,iscoreB);
	m_scorePublisher.RaiseEvent(&ScoreObserver::ScoreEvent,score);

}




void ScoreManager::CollisionEvent(const CollisionData& data){

		checkCollision(data.getObjectIdA(),data.getObjectIdB());
}

void ScoreManager::checkCollision(const ObjectId& idCollisionA,const ObjectId& idCollisionB ){

	if ( idCollisionA == objectID_LEFTWALL || idCollisionB == objectID_LEFTWALL)
	{
		if (idCollisionA == objectID_BALL || idCollisionB == objectID_BALL)
		{

			
			
		}

	}else{

		if ( idCollisionA == objectID_RIGHTWALL || idCollisionB == objectID_RIGHTWALL)
		{
			if (idCollisionA == objectID_BALL || idCollisionB == objectID_BALL)
			{


			}
		}
	}

	return;
	/*GameObjectSystem& gameObjectSystem= GameObjectSystem::GetSingleton();
	const ObjectPropertyTable* opt = gameObjectSystem.getProperties(Position::POSITION_ID);
	
	for (auto iter = opt->cbegin() ; iter!=opt->cend() ; ++iter)
	{
		const Position* posProp = static_cast<const Position*>(iter->second);

		const ObjectId& objectId = posProp->getObjectId();


	}*/


}


