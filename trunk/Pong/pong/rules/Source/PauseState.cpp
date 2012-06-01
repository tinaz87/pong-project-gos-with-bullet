#include "PauseState.h"
#include "GameObjectSystem.h"
#include "KeyboardInputManager.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"

PauseState::PauseState(const ObjectId& stateId)
	:FSMState(stateId)
{
	
}

void PauseState::onEnter()
{

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	cmp->SetActiveStatus(false);

	cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

	cmp->SetActiveStatus(false);

}

void PauseState::onLeave()
{
	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	cmp->SetActiveStatus(true);

	cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

	cmp->SetActiveStatus(true);
}
