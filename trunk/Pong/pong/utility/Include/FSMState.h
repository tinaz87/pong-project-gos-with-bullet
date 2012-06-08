#pragma once
#include "ObjectId.h"
#include "Containers.h"
#include "Publisher.h"
#include "TransitionObserver.h" // per poter essere in ascolto di cambiamenti di transizioni

typedef MvMap<ObjectId, ObjectId> TransitionMap;
class FSMState
{
public:
	FSMState(const ObjectId& stateId);
	~FSMState();
	const ObjectId&			getStateId() const;
	void					addTransition(const ObjectId& transitionId, const ObjectId& stateId);
	void					fireTransition(const ObjectId& transition);
	virtual const ObjectId& doTransition(const ObjectId& transitionId);
	virtual void			onEnter()= 0;
	virtual void			onLeave()= 0;
	virtual void			onFrame(real frametime, real timestep)= 0;
	Publisher<TransitionObserver>& getTransitionPublisher();
private:
	ObjectId						m_stateId;
	TransitionMap					m_transitionMap;
	Publisher<TransitionObserver>	m_transitionPublisher;
};
