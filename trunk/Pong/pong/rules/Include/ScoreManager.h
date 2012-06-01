#include "collisionObserver.h"
#include "ScoreObserver.h"
#include "Publisher.h"	// TransitionEvent
#include "Subscriber.h"	// CollisionEvent
#include "gameRulesComponent.h"


class ScoreManager: CollisionObserver
{

public:
	ScoreManager(const ObjectId& iobjectID_Ball,const ObjectId& iobjectID_Left,const ObjectId& iobjectID_Right);
	virtual void CollisionEvent(const CollisionData& data);
	void checkCollision(const ObjectId& idCollisionA,const ObjectId& idCollisionB );
	void fireScore(const unsigned int iscoreA,const unsigned int iscoreB);

private:
	
	const ObjectId& objectID_BALL;
	const ObjectId& objectID_LEFTWALL;
	const ObjectId& objectID_RIGHTWALL;

	SubscriberHelper<CollisionObserver> m_collisionSubscriber;
	Publisher<ScoreObserver> m_scorePublisher;


};