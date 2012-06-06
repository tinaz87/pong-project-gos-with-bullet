#include "PauseState.h"
#include "GameObjectSystem.h"
//#include "KeyboardInputManager.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"

const ObjectId PauseState::PAUSE_STATE_TEXT = "PAUSE_STATE_ID";

PauseState::PauseState(const ObjectId& stateId)
	:FSMState(stateId),pauseText(NULL)
{
	
}

void PauseState::onEnter()
{

	if( !pauseText ){

		pauseText = MV_NEW GfxInterfaceText();

		pauseText->rect = InterfaceRectangle(300,350,300,200);

		pauseText->text = "Pausa... Premi P per tornare in gioco...";

		ObjectProperty* prop = GameObjectSystem::GetSingleton().editProperty(GfxInterface::INTERFACE_PROPERTY_ID,GfxInterface::INTERFACE_PROPERTY_OBJ_ID);

		if (prop != NULL)
		{
			GfxInterface* gfxInterface = static_cast<GfxInterface*>(prop);

			gfxInterface->addText(PAUSE_STATE_TEXT,pauseText);

			
		}

	}

	pauseText->active = true;

	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

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

	pauseText->active = false;
}
