#pragma once
#include "ObjectId.h"
#include "Containers.h"
#include "Publisher.h"
#include "TransitionObserver.h"

class FSMState;

typedef MvMap<ObjectId, FSMState*> StateMap;
class FiniteStateMachine: public TransitionObserver
{
public:
	FiniteStateMachine(const ObjectId& startStateId);
	~FiniteStateMachine();
	void addState(FSMState* state);
	void updateState(const ObjectId& transitionId);
	void onFrame(real frametime, real timestep);
	void reset();
	virtual void TransitionEvent(const TransitionObserverData& data);

private:
	StateMap	m_stateMap;
	FSMState*	m_currentState;
	ObjectId	m_startStateId;
	SubscriberHelper<TransitionObserver> m_transitionSubscriber;
};