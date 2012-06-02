#include "StartState.h"
#include "GameObjectSystem.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "position.h"

StartState::StartState(const ObjectId& stateId)
	:FSMState(stateId)
{

}

void StartState::onEnter()
{

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();
	
	const ObjectPropertyTable* opt = gameobject.getProperties(Position::POSITION_ID);

	Position* position = static_cast<Position*>(opt->at(StringHash("ball1").GetHash()));

	quaternion initRotation= QuaternionFromRotationMatrix(MAT_IDENTITY);
	matrix matPos= MatrixPosition( vector3(0.f, 0.f, 0.5f), initRotation);

	position->setPosition(matPos);
	
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