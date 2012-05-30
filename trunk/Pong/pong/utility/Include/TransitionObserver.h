#pragma once
#include "ObjectId.h"

class TransitionObserverData
{
public:
	TransitionObserverData(const ObjectId& transitionId);
	void setTransition(const ObjectId& transitionId);
	const ObjectId& getTransition() const;
private:
	ObjectId m_transition;
};

class TransitionObserver
{
public:
	virtual ~TransitionObserver(){}
	virtual void TransitionEvent(const TransitionObserverData& data)= 0;
};