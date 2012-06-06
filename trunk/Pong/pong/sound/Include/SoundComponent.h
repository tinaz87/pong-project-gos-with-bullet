#pragma once
#include "Component.h"
#include "Allocator.h"
#include "CollisionObserver.h"
#include "Subscriber.h"
#include "SfxEffect.h"


typedef MvVector<SfxEffect*>	vectorSfxEffect;
typedef MvVector<SfxEffect*>::iterator iteratorSfxEffect;
typedef MvVector<SfxEffect*>::const_iterator const_iteratorSfxEffect;

class SoundComponent:public Component,public CollisionObserver
{
public:
	static const StringHash SOUND_COMPONENT_ID;
	SoundComponent();
	~SoundComponent();
	virtual void update(real frametime, real timestep);

	virtual void CollisionEvent(const CollisionData& data);

	void setCollisionPublisher(Publisher<CollisionObserver>* publisher);

private:

	vectorSfxEffect		 m_sfxEffectVector;


	SubscriberHelper<CollisionObserver> m_subscriberCollisionObserver;
};