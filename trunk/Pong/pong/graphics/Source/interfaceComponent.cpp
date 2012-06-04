
#include "GameObjectSystem.h"
#include "interfaceComponent.h"
#include "ScoreManager.h"

const StringHash InterfaceComponent::INTERFACE_COMPONENT_ID= StringHash("InterfaceComponent");

InterfaceComponent::InterfaceComponent():Component(INTERFACE_COMPONENT_ID,3),m_pd3dDevice(NULL)
	,scoreData(0,0)
													
{
	m_subscribeScoreObserver.SetSubscriber(this);
}

void InterfaceComponent::update(real frametime, real timestep){

		if (!m_pd3dDevice){

			GraphicsComponent* graphics = static_cast<GraphicsComponent*>(GameObjectSystem::GetSingleton().editComponent(GraphicsComponent::GRAPHICS_COMPONENT_ID));
			m_pd3dDevice = graphics->getDevice();
			D3DXCreateFont( m_pd3dDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );

		}

		m_pd3dDevice->BeginScene();
		displayText();
		m_pd3dDevice->EndScene();

		// Present the backbuffer contents to the display
		m_pd3dDevice->Present( NULL, NULL, NULL, NULL );

}


void InterfaceComponent::setScorePublisher(ScoreManager* publisher){

	m_subscribeScoreObserver.Subscribe(&(publisher->getScorePublisher()));

}


void InterfaceComponent::ScoreEvent(const ScoreData& iscore){


	scoreData = iscore;	

}



void InterfaceComponent::displayText()
{

	// Create a colour for the text - in this case blue
	D3DCOLOR fontColor = D3DCOLOR_ARGB(255,0,0,255);    

	// Create a rectangle to indicate where on the screen it should be drawn

	rct.top=20;
	rct.left=20;
	rct.right=780;
	rct.bottom=rct.top+20;	
	
	WCHAR sz[100];

	swprintf_s(sz, L"Points: %d", scoreData.getScoreA() );
	// Draw some text 
	m_font->DrawText(NULL, sz , -1, &rct, 0, fontColor );

	rct.left=700;

	swprintf_s(sz, L"Points: %d", scoreData.getScoreB() );
	//// Draw some text 
	m_font->DrawText(NULL, sz , -1, &rct, 0, fontColor );
}

InterfaceComponent::~InterfaceComponent(){}