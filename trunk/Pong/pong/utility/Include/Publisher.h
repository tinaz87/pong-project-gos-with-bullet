#pragma once
#include "Containers.h"

template<class SubscriberImpl> class SubscriberHelper;

template<class SubscriberImpl> class Publisher
{
public:
	Publisher(size_t subscribersUpperBound = 8);
	virtual		~Publisher();
	void		SetSubscribersUpperBound( size_t );
	
	void		RaiseEvent(void (SubscriberImpl::*Event)());
	template <class ParamType> 
	void		RaiseEvent(void (SubscriberImpl::*Event)(const ParamType& arg), const ParamType& param);

private:
	friend class		SubscriberHelper<SubscriberImpl>;

	typedef MvVector<SubscriberHelper<SubscriberImpl>*>	SubscribersVector;
	typedef typename SubscribersVector::iterator	SubscriberIterator;

	void				Subscribe(SubscriberHelper<SubscriberImpl>* subscriber);
	void				Unsubscribe(SubscriberHelper<SubscriberImpl>* subscriber);
	void				Unsubscribe(SubscriberIterator i);

	SubscribersVector	m_subscribers;
};

#include "Publisher.inl"