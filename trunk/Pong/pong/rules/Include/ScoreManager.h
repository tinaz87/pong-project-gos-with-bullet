#include "collisionObserver.h"
#include "ScoreObserver.h"
#include "Publisher.h"	// TransitionEvent
#include "Subscriber.h"	// CollisionEvent
//#include "gameRulesComponent.h"
#include "physicsComponent.h"
#include "Singleton.h"

class ScoreManager: public CollisionObserver, public Singleton<ScoreManager>
{

public:
	
	virtual void CollisionEvent(const CollisionData& data);
	void checkCollision(const ObjectId& idCollisionA,const ObjectId& idCollisionB );
	void fireScore(const unsigned int iscoreA,const unsigned int iscoreB);

	Publisher<ScoreObserver>& getScorePublisher();

	void setCollisionPublisher(Publisher<CollisionObserver>* publisher);
	
	virtual ~ScoreManager();

	static void  CreateInstance(const ObjectId ball,const ObjectId leftWall,const ObjectId rightWall);

	void		 reset();

private:
	
	ScoreManager(const ObjectId ball,const ObjectId leftWall,const ObjectId rightWall);

	ScoreManager();

	ObjectId objectID_BALL;
	ObjectId objectID_LEFTWALL;
	ObjectId objectID_RIGHTWALL;

	unsigned int scoreLeftPlayer;
	unsigned int scoreRightPlayer;

	Publisher<ScoreObserver> m_scorePublisher;

	SubscriberHelper<CollisionObserver> m_collisionSubscriber;

	


};