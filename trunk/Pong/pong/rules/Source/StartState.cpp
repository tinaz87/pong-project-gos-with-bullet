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
	
	/*const ObjectPropertyTable* opt = gameobject.getProperties(Position::POSITION_ID);

	Position* position = static_cast<Position*>(opt->at(StringHash("ball1").GetHash()));*/

	PhysicsBody* phyBody = static_cast<PhysicsBody*>(gameobject.editProperty(PhysicsBody::PHY_BODY_ID,ObjectId("ball1")));

	btRigidBody* rib = phyBody->editBody();

	(rib->getWorldTransform()).setOrigin(btVector3(0.f, 0.f, 0.5f));

	//quaternion initRotation= QuaternionFromRotationMatrix(MAT_IDENTITY);
	//matrix matPos= MatrixPosition(vector3(0.f, 0.f, 0.5f), initRotation);
	//position->setPosition(matPos);


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