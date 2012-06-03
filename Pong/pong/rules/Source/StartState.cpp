#include "StartState.h"
#include "GameState.h"
#include "GameObjectSystem.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "physicsBody.h"


StartState::StartState(const ObjectId& stateId)
	:FSMState(stateId)
{

}

void StartState::onEnter()
{

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	PhysicsBody* phyBody = static_cast<PhysicsBody*>(gameobject.editProperty(PhysicsBody::PHY_BODY_ID,ObjectId("ball1")));

	btRigidBody* ballBody = phyBody->editBody();

	// Reset ball velocity and position 
	if (rand()%2==0)
	{
		ballBody->setLinearVelocity(btVector3(100, 35.0f - rand() % 70,0));

	}else
		ballBody->setLinearVelocity(btVector3(-100, 35.0f - rand() % 70,0));
	
	ballBody->setAngularVelocity(btVector3(0,0,0));
	ballBody->setInterpolationWorldTransform(btTransform(btMatrix3x3::getIdentity(),btVector3(0,0.f,0.5f)));
	ballBody->setCenterOfMassTransform(btTransform(btMatrix3x3::getIdentity(),btVector3(0,0.f,0.5f)));

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	PhysicsComponent* phyCmp = static_cast<PhysicsComponent*>(cmp);

	btDiscreteDynamicsWorld* world = phyCmp->getWorld();

	// reset ball forces and others
	world->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(ballBody->getBroadphaseProxy(),world->getDispatcher());

	// disable component
	cmp->SetActiveStatus(false);

	cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

	cmp->SetActiveStatus(false);

}

void StartState::onLeave()
{
	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	cmp->SetActiveStatus(true);

	cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

	cmp->SetActiveStatus(true);
		
	// reset speed in gameState
	GameState::setSpeed(100);



}