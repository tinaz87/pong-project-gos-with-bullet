#pragma once
#include "graphicsComponent.h"
#include "ScoreObserver.h"
#include "TransitionObserver.h"
#include "Subscriber.h"


class ScoreManager;

class InterfaceComponent: public Component,public ScoreObserver,public TransitionObserver
{
public:
	static const StringHash INTERFACE_COMPONENT_ID;
	InterfaceComponent();
	~InterfaceComponent();
	virtual void update(real frametime, real timestep);

	virtual void ScoreEvent(const ScoreData& score);

	void setScorePublisher(ScoreManager* publisher);
	void setTransitionPublisher(ScoreManager* publisher);
private:
	
	void displayText();

	LPDIRECT3DDEVICE9		 m_pd3dDevice;				// Our rendering device
	LPD3DXFONT m_font;

	ScoreData scoreData;

	RECT rct;
	//std::wstring score;

	SubscriberHelper<ScoreObserver> m_subscribeScoreObserver;
	SubscriberHelper<TransitionObserver> m_subscribeTransitionObserver;
};