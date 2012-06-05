#include "PositionControllerComponent.h"
#include "GameObjectSystem.h"
#include "KeyboardInputManager.h"
#include "PositionControllerProperty.h"
#include "ComboControllerProperty.h"
#include "position.h"
#include "physicsBody.h"

const StringHash PositionControllerComponent::POSITION_CONTROLLER_ID= StringHash("posController");

PositionControllerComponent::PositionControllerComponent()
	:Component(POSITION_CONTROLLER_ID, 0),oldSpeed(0.0f),powerShoot(false)
{
	m_subscribeCollisionObserver.SetSubscriber(this);
}

PositionControllerComponent::~PositionControllerComponent()
{
}

void PositionControllerComponent::CollisionEvent(const CollisionData& data){

	bool shoot = false;

	ObjectProperty* ballProp= GameObjectSystem::GetSingleton().editProperty(PhysicsBody::PHY_BODY_ID, "ball1");

	ObjectProperty* bump1Prop= GameObjectSystem::GetSingleton().editProperty(PhysicsBody::PHY_BODY_ID, "bump1");

	ObjectProperty* bump2Prop= GameObjectSystem::GetSingleton().editProperty(PhysicsBody::PHY_BODY_ID, "bump2");

	if(ballProp != NULL && bump1Prop != NULL && bump2Prop != NULL){

		if( bump1Prop->getObjectId() == data.getObjectIdA() || bump1Prop->getObjectId() == data.getObjectIdB() )
		{

			if((ballProp->getObjectId() == data.getObjectIdA() || ballProp->getObjectId() == data.getObjectIdB())){

				ObjectProperty* comboProperty= GameObjectSystem::GetSingleton().editProperty(ComboControllerProperty::COMBO_CONTROLLER_PROPERTY_ID, bump1Prop->getObjectId());

				if(comboProperty!=NULL){
					ComboControllerProperty* combo=static_cast<ComboControllerProperty*>(comboProperty);

					if(combo->isCompleted())
					{
						shoot = true;
						if(!powerShoot)
							oldSpeed = (static_cast<PhysicsBody*>(ballProp))->getSpeed();

					}else{
						// reset velocita
						shoot = false;
						powerShoot = false;

						if(oldSpeed > 0.0f)
							(static_cast<PhysicsBody*>(ballProp))->setSpeed(oldSpeed);


					}

				}

			}
		}

		if( bump2Prop->getObjectId() == data.getObjectIdA() || bump2Prop->getObjectId() == data.getObjectIdB() )
		{

			if((ballProp->getObjectId() == data.getObjectIdA() || ballProp->getObjectId() == data.getObjectIdB())){

				ObjectProperty* comboProperty= GameObjectSystem::GetSingleton().editProperty(ComboControllerProperty::COMBO_CONTROLLER_PROPERTY_ID, bump2Prop->getObjectId());

				if(comboProperty!=NULL){
					ComboControllerProperty* combo=static_cast<ComboControllerProperty*>(comboProperty);

					if(combo->isCompleted())
					{
						shoot = true;

						if(!powerShoot)
							oldSpeed = (static_cast<PhysicsBody*>(ballProp))->getSpeed();

					}else{


						// reset velocita
						shoot = false;
						powerShoot = false;

						if(oldSpeed > 0.0f)
							(static_cast<PhysicsBody*>(ballProp))->setSpeed(oldSpeed);

					}

				}


			}
		}

		if (shoot){

			PhysicsBody* m_ballBody= static_cast<PhysicsBody*>(ballProp);
			powerShoot = true;
			m_ballBody->setSpeed(50);

		}


	}

}

void PositionControllerComponent::update(real frametime, real timestep)
{
	GameObjectSystem& gameObjectSystem= GameObjectSystem::GetSingleton();

	const ObjectPropertyTable* opt = gameObjectSystem.getProperties(PositionControllerProperty::POSITION_CONTROLLER_PROPERTY_ID);	

	for ( auto iter = opt->cbegin() ; iter!=opt->cend() ; ++iter )
	{
		const PositionControllerProperty* posControlProp = static_cast<const PositionControllerProperty*>(iter->second);

		ObjectProperty* positionProperty= gameObjectSystem.editProperty(Position::POSITION_ID, posControlProp->getObjectId());

		if ( positionProperty != nullptr)
		{
			Position* position = static_cast<Position*>(positionProperty);

			matrix matPos = position->getPosition();

			ObjectProperty* comboProperty= gameObjectSystem.editProperty(ComboControllerProperty::COMBO_CONTROLLER_PROPERTY_ID, posControlProp->getObjectId());

			if(comboProperty != NULL){


				ComboControllerProperty* comboControllerProperty = static_cast<ComboControllerProperty*>(comboProperty);

				comboControllerProperty->updateTimer(frametime);

				if( KeyboardInputManager::GetSingleton().getKeyState(comboControllerProperty->getFirstComboKey()) > 0  ){

					comboControllerProperty->startCombo();

				}else{

					if( comboControllerProperty->isInProgress() && KeyboardInputManager::GetSingleton().getKeyState(comboControllerProperty->getSecondComboKey()) > 0  ){

						comboControllerProperty->setComboComplete(true);

					}

				}


			}

			if( KeyboardInputManager::GetSingleton().getKeyState(posControlProp->getUp()) == 0 || KeyboardInputManager::GetSingleton().getKeyState(posControlProp->getDown()) == 0 )
			{ // 0 se è rilasciato 1 se è premuto

				vector3 translation;
				GetTranslation(matPos,&translation);

				if( KeyboardInputManager::GetSingleton().getKeyState(posControlProp->getUp()) > 0 && translation.y > -82 ){

					AddTranslation(&matPos,vector3(0.0f,-1.0f,0.0f));
					position->setPosition(matPos);


					if(comboProperty != NULL){

						ComboControllerProperty* comboControllerProperty = static_cast<ComboControllerProperty*>(comboProperty);

						comboControllerProperty->stopCombo();
					}


				}else{

					if( KeyboardInputManager::GetSingleton().getKeyState(posControlProp->getDown()) > 0 && translation.y < 82 ){

						AddTranslation(&matPos,vector3(0.0f,1.0f,0.0f));
						position->setPosition(matPos);


						if(comboProperty != NULL){

							ComboControllerProperty* comboControllerProperty = static_cast<ComboControllerProperty*>(comboProperty);

							comboControllerProperty->stopCombo();
						}
					}

				}

			}

		}

	}	
}

void PositionControllerComponent::setCollisionPublisher(Publisher<CollisionObserver>* publisher){

	m_subscribeCollisionObserver.Subscribe(publisher);

}