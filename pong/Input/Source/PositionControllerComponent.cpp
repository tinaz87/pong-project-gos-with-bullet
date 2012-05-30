#include "PositionControllerComponent.h"
#include "GameObjectSystem.h"
#include "KeyboardInputManager.h"
#include "PositionControllerProperty.h"
#include "position.h"

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
		
		if (positionProperty!=nullptr)
		{
			Position* position = static_cast<Position*>(positionProperty);

			matrix matPos = position->getPosition();

			if( KeyboardInputManager::GetSingleton().getKeyState(posControlProp->getUp()) == 0 || KeyboardInputManager::GetSingleton().getKeyState(posControlProp->getDown()) == 0 )
			{ // 0 se � rilasciato 1 se � premuto
			
				vector3 translation;
				GetTranslation(matPos,&translation);

				if( KeyboardInputManager::GetSingleton().getKeyState(posControlProp->getUp()) > 0 && translation.y > -82 ){

						AddTranslation(&matPos,vector3(0.0f,-1.0f,0.0f));
						position->setPosition(matPos);
				}else{

					if( KeyboardInputManager::GetSingleton().getKeyState(posControlProp->getDown()) > 0 && translation.y < 82 ){

							AddTranslation(&matPos,vector3(0.0f,1.0f,0.0f));
							position->setPosition(matPos);
					}

				}

			}

		}

	}

}
