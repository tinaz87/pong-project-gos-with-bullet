#include "EndState.h"
#include "GameObjectSystem.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "ScoreManager.h"

EndState::EndState(const ObjectId& stateId)
	:FSMState(stateId)
{

}

void EndState::onEnter()
{

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);	

	if (cmp != NULL){
		
		cmp->SetActiveStatus(false);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(false);
		
	}

	SetMessageStatusActive(true,gameobject);

}


void EndState::onLeave(){

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	if (cmp != NULL){

		PhysicsComponent* phyCmp = static_cast<PhysicsComponent*>(cmp);
		phyCmp->reset();

		ScoreManager::GetSingleton().reset();

		cmp->SetActiveStatus(true);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(true);

		
	}

	SetMessageStatusActive(false,gameobject);
}

void EndState::SetMessageStatusActive(const bool status,GameObjectSystem& gameobject){



	ObjectProperty* op = gameobject.editProperty(GfxFont::GFX_FONT_ID,ObjectId("EndGameState_1_ID"));
	
	if(op!=NULL){

		GfxFont* pfont = static_cast<GfxFont*>(op);

		pfont->setActive(status);
	}
	
	op = gameobject.editProperty(GfxFont::GFX_FONT_ID,ObjectId("EndGameState_2_ID"));

	if(op!=NULL){

		GfxFont* pfont = static_cast<GfxFont*>(op);

		pfont->setActive(status);
	}
}