#include "collisionObserver.h"
#include "ScoreObserver.h"
#include "Publisher.h"	// TransitionEvent
#include "Subscriber.h"	// CollisionEvent
//#include "gameRulesComponent.h"
#include "physicsComponent.h"


class ScoreManager: CollisionObserver
{

public:

	ScoreManager(const std::string& ballId,const std::string& bumperIdLeft,const std::string& bumperIdRight);
	virtual void CollisionEvent(const CollisionData& data);
	void checkCollision(const ObjectId& idCollisionA,const ObjectId& idCollisionB );
	void fireScore(const unsigned int iscoreA,const unsigned int iscoreB);

	Publisher<ScoreObserver>& getScorePublisher();

	void setCollisionPublisher(PhysicsComponent* publisher);
	
private:
	
	const ObjectId objectID_BALL;
	const ObjectId objectID_LEFTWALL;
	const ObjectId objectID_RIGHTWALL;

	unsigned int scoreLeftPlayer;
	unsigned int scoreRightPlayer;

	Publisher<ScoreObserver> m_scorePublisher;

	SubscriberHelper<CollisionObserver> m_collisionSubscriber;

	


};