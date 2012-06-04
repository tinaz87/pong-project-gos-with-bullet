#pragma once
#include "Containers.h"

template<class SubscriberImpl> class Publisher;

template<class SubscriberImpl> 
class SubscriberHelper
{
public:
	SubscriberHelper(SubscriberImpl* subscriber= 0);
	~SubscriberHelper();

	SubscriberImpl*	GetSubscriber();
	void			SetSubscriber(SubscriberImpl* subscriber);

	void			Subscribe(Publisher<SubscriberImpl>* publisher);
	void			Unsubscribe();

	bool			IsSubscribed() const;

protected:	
	friend class	Publisher<SubscriberImpl>;

	void						UnsubscribedCallback();

	typedef MvVector<Publisher<SubscriberImpl>*>	PublisherVector;
	typedef typename PublisherVector::iterator		PublisherIterator;

	PublisherVector				m_publisher;
	SubscriberImpl*				m_subscriberImpl;	
};

#include "Subscriber.inl"