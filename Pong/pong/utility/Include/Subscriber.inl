#include "glog/logging.h"
#include "Publisher.h"
#include <algorithm>

template<class SubscriberImpl> 
SubscriberHelper<SubscriberImpl>::SubscriberHelper(SubscriberImpl* subscriberImpl)
	: m_subscriberImpl(subscriberImpl)//,m_publisher()
{
}

template<class SubscriberImpl> 
SubscriberHelper<SubscriberImpl>::~SubscriberHelper()
{
	Unsubscribe();
}

template<class SubscriberImpl>
inline void SubscriberHelper<SubscriberImpl>::Subscribe(Publisher<SubscriberImpl> *publisher) 
{
	DLOG_ASSERT(m_subscriberImpl);

	Publisher<SubscriberImpl*> findPublisher = *(find(PublisherIterator.begin(),PublisherIterator.end(),publisher));

	if (m_subscriberImpl && findPublisher != PublisherIterator->end())//m_publisher != publisher)
	{
		//Unsubscribe();
		m_publisher.push_back(publisher);

		if (publisher)
		{
			publisher->Subscribe(this);
		}
	}
}

template<class SubscriberImpl>
inline void SubscriberHelper<SubscriberImpl>::Unsubscribe() 
{
	for(PublisherIterator it = PublisherIterator.begin();it!=PublisherIterator.end();++it){

		/*	if (m_publisher)
		{
		m_publisher->Unsubscribe(this);
		m_publisher = NULL;
		}*/

		if (*it)
		{
			(*it)->Unsubscribe(this);
			m_publisher.erase(it);
		}
	}
}

template<class SubscriberImpl>
inline void SubscriberHelper<SubscriberImpl>::SetSubscriber(SubscriberImpl* subscriberImpl)
{
	if (subscriberImpl == NULL && m_publisher != NULL)
	{
		Unsubscribe();
	}

	m_subscriberImpl = subscriberImpl;
}

template<class SubscriberImpl>
bool SubscriberHelper<SubscriberImpl>::IsSubscribed() const
{
	return m_publisher != NULL;
}

template<class SubscriberImpl>
inline SubscriberImpl* SubscriberHelper<SubscriberImpl>::GetSubscriber() 
{
	return m_subscriberImpl;
}

template<class SubscriberImpl>
void SubscriberHelper<SubscriberImpl>::UnsubscribedCallback() 
{
	m_publisher = NULL;
}
