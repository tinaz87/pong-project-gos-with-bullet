#include "StartState.h"
#include "physicsComponent.h"
#include "PositionControllerComponent.h"
#include "physicsBody.h"
#include "GameObjectSystem.h"



StartState::StartState(const ObjectId& stateId)
	:FSMState(stateId),m_main(NULL),m_second(NULL)
{
	
	ObjectProperty* op = GameObjectSystem::GetSingleton().editProperty(GfxFont::GFX_FONT_ID,ObjectId("CENTER_MAIN_ID"));

	if(op!=NULL){

		m_main = static_cast<GfxFont*>(op);

	}

	op = GameObjectSystem::GetSingleton().editProperty(GfxFont::GFX_FONT_ID,ObjectId("CENTER_SECOND_ID"));

	if(op!=NULL){

		m_second = static_cast<GfxFont*>(op);

	}
}


StartState::~StartState(){
	

}

void StartState::onEnter()
{
	GameObjectSystem& gameobject = GameObjectSystem::GetSingleton();

	m_main->setText("START GAME");
	m_second->setText("    Premi i per Iniziare");

	m_main->setActive(true);
	m_second->setActive(true);


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

	m_main->setActive(false);
	m_second->setActive(false);

	Component* cmp = gameobject.editComponent(PhysicsComponent::PHYSICS_COMPONENT_ID);

	if (cmp != NULL){


		cmp->SetActiveStatus(true);

		cmp = gameobject.editComponent(PositionControllerComponent::POSITION_CONTROLLER_ID);

		if (cmp!= NULL)
			cmp->SetActiveStatus(true);

		
	}

}