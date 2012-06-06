#include "gameRulesComponent.h"
#include "GameObjectSystem.h"
#include "StartState.h"
#include "GameState.h"
#include "PauseState.h"
#include "EndState.h"
#include "KeyboardInputManager.h"
#include "ScoreManager.h"

#define POINTS 2


const StringHash GameRulesComponent::RULES_COMPONENT_ID= StringHash("Rules");

//States
const ObjectId GameRulesComponent::START_STATE= ObjectId("StartState");
const ObjectId GameRulesComponent::GAME_STATE= ObjectId("GameState");
const ObjectId GameRulesComponent::END_STATE= ObjectId("EndState");
const ObjectId GameRulesComponent::PAUSE_STATE= ObjectId("PauseState");


//transitions
const ObjectId GameRulesComponent::KEY_START_PRESSED= ObjectId("StartKey");
const ObjectId GameRulesComponent::KEY_PAUSE_RELEASED= ObjectId("PauseKey");
const ObjectId GameRulesComponent::KEY_RESTART= ObjectId("RestartKey");
const ObjectId GameRulesComponent::SCORE_REACHED= ObjectId("Score");
const ObjectId GameRulesComponent::GOAL_SCORED= ObjectId("Goal");

GameRulesComponent::GameRulesComponent()
	:Component(RULES_COMPONENT_ID, 3)
	,FiniteStateMachine(START_STATE)
{
	//TODO: sistemare il problema di come settare il ppublisher al gamestate
	m_subscribeScoreObserver.SetSubscriber(this);
	m_keyPause= false;
	configureFSM();
}

GameRulesComponent::~GameRulesComponent()
{

}


void GameRulesComponent::ScoreEvent(const ScoreData& score){

	ObjectId idTransition;
	if ( score.getScoreA() < POINTS && score.getScoreB() < POINTS )
		idTransition = GOAL_SCORED;
	else
		idTransition = SCORE_REACHED;

	TransitionObserverData data(idTransition);	

	
	TransitionEvent(data);

}

void GameRulesComponent::update(real frametime, real timestep)
{
	if(KeyboardInputManager::GetSingleton().getKeyState('I'))
	{
		FiniteStateMachine::updateState(KEY_START_PRESSED);
	}
	else if(KeyboardInputManager::GetSingleton().getKeyState('P'))
	{
		m_keyPause= true;
	}
	else if(m_keyPause && KeyboardInputManager::GetSingleton().getKeyState('P')==0)
	{
		FiniteStateMachine::updateState(KEY_PAUSE_RELEASED);
		m_keyPause= false;
	}
	else if(KeyboardInputManager::GetSingleton().getKeyState('R'))
	{
		FiniteStateMachine::updateState(KEY_RESTART);
	}
	else
	{
		FiniteStateMachine::updateState("");
	}

	FiniteStateMachine::onFrame(frametime, timestep);

}

void GameRulesComponent::setScorePublisher(Publisher<ScoreObserver>* publisher){

	m_subscribeScoreObserver.Subscribe(publisher);

}

void GameRulesComponent::configureFSM()
{
	StartState* start= MV_NEW StartState(START_STATE);
	start->addTransition(KEY_START_PRESSED, GAME_STATE);

	GameState* game= MV_NEW GameState(GAME_STATE);
	game->addTransition(KEY_PAUSE_RELEASED, PAUSE_STATE);
	game->addTransition(SCORE_REACHED, END_STATE);
	game->addTransition(GOAL_SCORED,START_STATE);

	PauseState* pause = MV_NEW PauseState(PAUSE_STATE);
	pause->addTransition(KEY_PAUSE_RELEASED, GAME_STATE);

	EndState* endState= MV_NEW EndState(END_STATE);
	endState->addTransition(KEY_RESTART, START_STATE);

	FiniteStateMachine::addState(pause);
	FiniteStateMachine::addState(start);
	FiniteStateMachine::addState(game);
	FiniteStateMachine::addState(endState);


}
