#include "FiniteStateMachine.h"
#include "glog\logging.h"
#include "FSMState.h"

FiniteStateMachine::FiniteStateMachine(const ObjectId& startStateId)
	:m_currentState(NULL)
	,m_startStateId(startStateId)
{
	m_transitionSubscriber.SetSubscriber(this); // implementa l'interfaccia per ascoltare le transizioni avendo implementato transitionevent
												// sottoscrive un ascoltatore di eventi...In questo caso di transizioni
}

void FiniteStateMachine::reset()
{
	m_currentState= m_stateMap[m_startStateId];
	DLOG_ASSERT(m_currentState!=NULL);
}

void FiniteStateMachine::TransitionEvent(const TransitionObserverData& data)
{
	updateState(data.getTransition());
}

void FiniteStateMachine::updateState(const ObjectId& transitionId)
{
	if(m_currentState != NULL)
	{
		const ObjectId& newStateId= m_currentState->doTransition(transitionId);
		if(m_currentState->getStateId() != newStateId)
		{
			m_currentState->onLeave();
			FSMState* newState= m_stateMap[newStateId];
			DLOG_ASSERT(newState!= NULL);
			newState->onEnter();
			m_currentState= newState;
		}
	}
	else
	{
		reset();
		m_currentState->onEnter();
	}
}

void FiniteStateMachine::onFrame(real frametime, real timestep) // per evitare di cambiare piu stati nel giro di un solo frame
{
	if(m_currentState != NULL)
	{
		m_currentState->onFrame(frametime, timestep);
	}
}

void FiniteStateMachine::addState(FSMState* state)
{
	m_transitionSubscriber.Subscribe(&state->getTransitionPublisher()); // Sottoscrivi un publisher di eventi
	m_stateMap[state->getStateId()]= state;
	
}

FiniteStateMachine::~FiniteStateMachine()
{
	for(StateMap::iterator it= m_stateMap.begin(); it != m_stateMap.end(); ++it)
	{
		MV_DELETE(it->second);
	}
	m_stateMap.clear();
}