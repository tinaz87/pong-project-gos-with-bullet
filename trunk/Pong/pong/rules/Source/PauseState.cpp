#include "PauseState.h"
#include "GameObjectSystem.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"



PauseState::PauseState(const ObjectId& stateId)
	:FSMState(stateId)
{
	
}

void PauseState::onEnter()
{
	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	SetMessageStatusActive(true,gameobject);

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	if (cmp != NULL){

		cmp->SetActiveStatus(false);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(false);
		
	}

}

void PauseState::onLeave()
{	

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	if (cmp != NULL){


		cmp->SetActiveStatus(true);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(true);

		
	}

	SetMessageStatusActive(false,gameobject);
	
}

void PauseState::SetMessageStatusActive(const bool status,GameObjectSystem& gameobject){

	ObjectProperty* op = gameobject.editProperty(GfxFont::GFX_FONT_ID,ObjectId("PauseState_1_ID"));
	
	if(op!=NULL){

		GfxFont* pfont = static_cast<GfxFont*>(op);

		pfont->setActive(status);
	}

	op = gameobject.editProperty(GfxFont::GFX_FONT_ID,ObjectId("PauseState_2_ID"));

	if(op!=NULL){

		GfxFont* pfont = static_cast<GfxFont*>(op);

		pfont->setActive(status);
	}

}