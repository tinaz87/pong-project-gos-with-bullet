#include "PositionControllerComponent.h"
#include "GameObjectSystem.h"
#include "KeyboardInputManager.h"
#include "PositionControllerProperty.h"
#include "ComboControllerProperty.h"
#include "position.h"
#include "physicsBody.h"

const StringHash PositionControllerComponent::POSITION_CONTROLLER_ID= StringHash("posController");

PositionControllerComponent::PositionControllerComponent()
	:Component(POSITION_CONTROLLER_ID, 0)
{

}

PositionControllerComponent::~PositionControllerComponent()
{
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
