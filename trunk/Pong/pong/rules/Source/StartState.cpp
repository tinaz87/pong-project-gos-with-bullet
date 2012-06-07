#include "StartState.h"
//#include "GameState.h"
#include "GameObjectSystem.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "physicsBody.h"
//#include "interfaceComponent.h"

const ObjectId StartState::START_STATE_TEXT_ID = "START_STATE_ID";

StartState::StartState(const ObjectId& stateId)
	:FSMState(stateId),startText(NULL)
{
	

}

void StartState::onEnter()
{

	if (!startText)
	{
		startText = MV_NEW GfxInterfaceText();

		startText->rect = InterfaceRectangle(290,325,250,200);

		startText->text = " --- Premi i Per iniziare --- ";

		startText->fontColor = D3DCOLOR_ARGB(255,255,255,255); 

		ObjectProperty* prop = GameObjectSystem::GetSingleton().editProperty(GfxInterface::INTERFACE_PROPERTY_ID,GfxInterface::INTERFACE_PROPERTY_OBJ_ID);

		if (prop != NULL)
		{
			GfxInterface* gfxInterface = static_cast<GfxInterface*>(prop);

			gfxInterface->addText(START_STATE_TEXT_ID,startText);
		}
		
		
	}

	startText->active = true;

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

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

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	if (cmp != NULL){


		cmp->SetActiveStatus(true);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(true);

		
	}

	startText->active = false;


}