#include "PauseState.h"
#include "GameObjectSystem.h"
#include "KeyboardInputManager.h"
#include "physicsComponent.h"

PauseState::PauseState(const ObjectId& stateId)
	:FSMState(stateId)
{

}

void PauseState::onEnter()
{
	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();
	
	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	cmp->SetActiveStatus(false);
}

void PauseState::onLeave()
{
	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	cmp->SetActiveStatus(true);
}
