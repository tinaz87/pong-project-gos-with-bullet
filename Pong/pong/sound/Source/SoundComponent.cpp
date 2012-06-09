#include "SoundComponent.h"
#include "GameObjectSystem.h"



const StringHash SoundComponent::SOUND_COMPONENT_ID= StringHash("SoundComponent");

SoundComponent::SoundComponent():Component(SOUND_COMPONENT_ID,3){

	//Initialize OpenAL
	alutInit(NULL, 0);

	GameObjectSystem& gameObjectSystem= GameObjectSystem::GetSingleton();
	const ObjectPropertyTable* sfxEffectMap =  gameObjectSystem.getProperties(SfxEffect::SFX_EFFECT_ID);



	if(sfxEffectMap!=NULL)
	{
		for(ObjectPropertyTable::const_iterator opIt= sfxEffectMap->begin(); opIt != sfxEffectMap->end(); ++opIt)
		{
			SfxEffect* sfxEffect= static_cast<SfxEffect*>(opIt->second);

			sfxEffect->PrepareAudio();
			m_sfxEffectVector.push_back(sfxEffect);
		}
	}


	m_subscriberCollisionObserver.SetSubscriber(this);
}

void SoundComponent::update(real frametime, real timestep){

	for(iteratorSfxEffect it = m_sfxEffectVector.begin(); it!=m_sfxEffectVector.end();++it){

		SfxEffect* sfx = *it;
		if(sfx->isToPlay())
			sfx->Play();

		if(sfx->isInPlay())
			sfx->Update(frametime);
	}

}

void SoundComponent::CollisionEvent(const CollisionData& data){	

	if(data.getObjectIdA() == ObjectId("bump1") || data.getObjectIdB() == ObjectId("bump1")){

		ObjectProperty* sfxProp = GameObjectSystem::GetSingleton().editProperty(SfxEffect::SFX_EFFECT_ID,ObjectId("bump1"));

		if (sfxProp != NULL){

			SfxEffect* sfx = static_cast<SfxEffect*>(sfxProp);
			sfx->setToPlay();

		}

	}else{

		if(data.getObjectIdA() == ObjectId("bump2") || data.getObjectIdB() == ObjectId("bump2")){

			ObjectProperty* sfxProp = GameObjectSystem::GetSingleton().editProperty(SfxEffect::SFX_EFFECT_ID,ObjectId("bump2"));

			if (sfxProp != NULL){

				SfxEffect* sfx = static_cast<SfxEffect*>(sfxProp);
				sfx->setToPlay();

			}

		}else{

			if(data.getObjectIdA() == ObjectId("wall3") || data.getObjectIdB() == ObjectId("wall3")){

				ObjectProperty* sfxProp = GameObjectSystem::GetSingleton().editProperty(SfxEffect::SFX_EFFECT_ID,ObjectId("wall3"));

				if (sfxProp != NULL){

					SfxEffect* sfx = static_cast<SfxEffect*>(sfxProp);
					sfx->setToPlay();

				}

			}else{

				if(data.getObjectIdA() == ObjectId("wall4") || data.getObjectIdB() == ObjectId("wall4")){

					ObjectProperty* sfxProp = GameObjectSystem::GetSingleton().editProperty(SfxEffect::SFX_EFFECT_ID,ObjectId("wall4"));

					if (sfxProp != NULL){

						SfxEffect* sfx = static_cast<SfxEffect*>(sfxProp);
						sfx->setToPlay();

					}

				}

			}

		}


	}

}

void SoundComponent::setCollisionPublisher(Publisher<CollisionObserver>* publisher){


	m_subscriberCollisionObserver.Subscribe(publisher);
}

SoundComponent::~SoundComponent(){

	m_sfxEffectVector.clear();

}