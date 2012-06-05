#include "StartState.h"
#include "GameState.h"
#include "GameObjectSystem.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "physicsBody.h"
#include "interfaceComponent.h"

const ObjectId StartState::START_STATE_TEXT_ID = "StartText";

StartState::StartState(const ObjectId& stateId)
	:FSMState(stateId),startText(nullptr)
{
	

}

void StartState::onEnter()
{

	if (!startText)
	{
		startText = MV_NEW GfxInterfaceText();

		startText->rect = InterfaceRectangle(100,100,200,200);

		startText->text = "START TEXT";

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

	if(cmp != NULL){
		PhysicsComponent* phyCmp = static_cast<PhysicsComponent*>(cmp);
		phyCmp->reset();

	}
	// disable component
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

	startText->active = false;
}