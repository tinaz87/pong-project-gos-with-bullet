#include "Subscriber.h"

template<class SubscriberImpl>
inline Publisher<SubscriberImpl>::Publisher(size_t subscribersUpperBound)
{
	m_subscribers.reserve(subscribersUpperBound);
}

template<class SubscriberImpl>
inline Publisher<SubscriberImpl>::~Publisher() 
{
	while(m_subscribers.size() > 0)
	{
		Unsubscribe(m_subscribers.begin());
	}
}

template<class SubscriberImpl>
inline void Publisher<SubscriberImpl>::Unsubscribe(SubscriberHelper<SubscriberImpl>* subscriber)
{ 
	for(SubscriberIterator it= m_subscribers.begin(); it != m_subscribers.end(); ++it)
	{
		if(*it == subscriber)
		{
			Unsubscribe(it);
			break;
		}
	}
}

template<class SubscriberImpl>
inline void Publisher<SubscriberImpl>::Unsubscribe(typename Publisher<SubscriberImpl>::SubscriberIterator it)
{
	SubscriberHelper<SubscriberImpl>* subscriber = *it;
	m_subscribers.erase(it);
	subscriber->UnsubscribedCallback();
}

template<class SubscriberImpl>
inline void Publisher<SubscriberImpl>::Subscribe(SubscriberHelper<SubscriberImpl>* subscriber) 
{ 
	m_subscribers.push_back(subscriber); 
}

template<class SubscriberImpl>
inline void Publisher<SubscriberImpl>::SetSubscribersUpperBound(size_t size)
{
	m_subscribers.reserve( size );
}

template <class SubscriberImpl>
template <class ParamType> 
inline void Publisher<SubscriberImpl>::RaiseEvent(void (SubscriberImpl::*Event)(const ParamType&),
	const ParamType& param)
{
	for (typename Publisher<SubscriberImpl>::SubscriberIterator it = m_subscribers.begin(); it != m_subscribers.end(); ++it)
	{
		( ((*it)->GetSubscriber() )->*Event)(param);
	}
}

template<class SubscriberImpl>
inline void Publisher<SubscriberImpl>::RaiseEvent(void (SubscriberImpl::*Event)())
{
	for (typename Publisher<SubscriberImpl>::SubscriberIterator it = m_subscribers.begin(); it != m_subscribers.end(); ++it)
	{
		( ((*it)->GetSubscriber() )->*Event)();
	}
}
