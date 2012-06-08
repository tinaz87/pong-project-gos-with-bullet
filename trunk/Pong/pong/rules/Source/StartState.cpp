#include "StartState.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "physicsBody.h"
#include "GameObjectSystem.h"



StartState::StartState(const ObjectId& stateId)
	:FSMState(stateId)
{
	

}


StartState::~StartState(){
	

}

void StartState::onEnter()
{
	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();


	SetMessageStatusActive(true,gameobject);


	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	if (cmp != NULL){
		
		PhysicsComponent* phyCmp = static_cast<PhysicsComponent*>(cmp);
		phyCmp->reset();

		cmp->SetActiveStatus(false);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(false);
		
	}

}

void StartState::onLeave()
{
	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	SetMessageStatusActive(false,gameobject);

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	if (cmp != NULL){


		cmp->SetActiveStatus(true);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(true);

		
	}

	/*startText->active = false;*/


}

void StartState::SetMessageStatusActive(const bool status,GameObjectSystem& gameobject){

	ObjectProperty* op = gameobject.editProperty(GfxFont::GFX_FONT_ID,ObjectId("StartState_1_ID"));
	
	if(op!=NULL){

		GfxFont* pfont = static_cast<GfxFont*>(op);

		pfont->setActive(status);
	}
	
	op = gameobject.editProperty(GfxFont::GFX_FONT_ID,ObjectId("StartState_2_ID"));

	if(op!=NULL){

		GfxFont* pfont = static_cast<GfxFont*>(op);

		pfont->setActive(status);
	}

}