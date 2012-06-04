#include "glog/logging.h"
#include "Publisher.h"

template<class SubscriberImpl> 
SubscriberHelper<SubscriberImpl>::SubscriberHelper(SubscriberImpl* subscriberImpl)
	: m_publisher(NULL)
	, m_subscriberImpl(subscriberImpl)
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

	if (m_subscriberImpl && m_publisher != publisher)
	{
		Unsubscribe();
		m_publisher = publisher;

		if (m_publisher)
		{
			m_publisher->Subscribe(this);
		}
	}
}

template<class SubscriberImpl>
inline void SubscriberHelper<SubscriberImpl>::Unsubscribe() 
{
	if (m_publisher)
	{
		m_publisher->Unsubscribe(this);
		m_publisher = NULL;
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
