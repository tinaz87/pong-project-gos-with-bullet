#include "FSMState.h"

FSMState::FSMState(const ObjectId& stateId)
	:m_stateId(stateId)
{

}

FSMState::~FSMState()
{

}

const ObjectId& FSMState::getStateId() const
{
	return m_stateId;
}

void FSMState::addTransition(const ObjectId& transitionId, const ObjectId& stateId)
{
	m_transitionMap[transitionId]= stateId;
}

const ObjectId& FSMState::doTransition(const ObjectId& transitionId)
{
	TransitionMap::iterator it= m_transitionMap.find(transitionId);
	if(it != m_transitionMap.end())
	{
		return it->second;
	}
	return m_stateId;
}

void FSMState::fireTransition(const ObjectId& transitionId)
{
	TransitionObserverData data(transitionId);
	m_transitionPublisher.RaiseEvent(&TransitionObserver::TransitionEvent, data);
}

Publisher<TransitionObserver>& FSMState::getTransitionPublisher()
{
	return m_transitionPublisher;
}
