#include "TransitionObserver.h"

TransitionObserverData::TransitionObserverData(const ObjectId& transitionId)
	:m_transition(transitionId)
{

}

void TransitionObserverData::setTransition(const ObjectId& transitionId)
{
	m_transition= transitionId;
}

const ObjectId& TransitionObserverData::getTransition() const
{
	return m_transition;
}

void TransitionObserver::TransitionEvent(const TransitionObserverData& data){
	
	if ( (data.getTransition()).GetHash() == StringHash("PauseState").GetHash() )
	{
		int a= 0;
	}

}
