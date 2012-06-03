#include "StartState.h"
#include "GameObjectSystem.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "position.h"
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


	// Reset ball position 
	if (rand()%2==0)
	{
		//ballBody->setLinearVelocity(btVector3(100,0,10));
	}else
		//ballBody->setLinearVelocity(btVector3(-100,0,-10));
	
	ballBody->setAngularVelocity(btVector3(0,0,0));

	ballBody->setInterpolationWorldTransform(btTransform(btMatrix3x3::getIdentity(),btVector3(0,0.f,0.5f)));
	ballBody->setCenterOfMassTransform(btTransform(btMatrix3x3::getIdentity(),btVector3(0,0.f,0.5f)));

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

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
}