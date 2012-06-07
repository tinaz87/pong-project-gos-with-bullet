#include "EndState.h"
#include "GameObjectSystem.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "ScoreManager.h"


const ObjectId EndState::END_STATE_TEXT = "END_STATE_TEXT_ID";
EndState::EndState(const ObjectId& stateId)
	:FSMState(stateId),endText(NULL)
{

}

void EndState::onEnter()
{

	if( !endText ){

		endText = MV_NEW GfxInterfaceText();

		endText->rect = InterfaceRectangle(250,325,400,200);

		endText->text = "The End ... Premi R per ricominciare...";

		endText->fontColor = D3DCOLOR_ARGB(255,255,0,255); 

		ObjectProperty* prop = GameObjectSystem::GetSingleton().editProperty(GfxInterface::INTERFACE_PROPERTY_ID,GfxInterface::INTERFACE_PROPERTY_OBJ_ID);

		if (prop != NULL)
		{
			GfxInterface* gfxInterface = static_cast<GfxInterface*>(prop);

			gfxInterface->addText(END_STATE_TEXT,endText);

			
		}

	}

	endText->active = true;	

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);	

	if (cmp != NULL){
		
		cmp->SetActiveStatus(false);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(false);
		
	}


}
void EndState::onLeave(){

	endText->active = false;

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

}