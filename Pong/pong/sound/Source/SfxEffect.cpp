#include "SfxEffect.h"


 const StringHash SfxEffect::SFX_EFFECT_ID = "SFX_EFFECT_ID";
 

 SfxEffect::SfxEffect(const ObjectId& objectId):ObjectProperty(SFX_EFFECT_ID,objectId),
												m_audio(NULL),m_toPlay(false)
 {


 }

 SfxEffect::~SfxEffect(){

	
	 delete m_audio;

 }
 

 void SfxEffect::setSound(const LPCWSTR fileName){

	 m_fileName = fileName; 

 }
 
 void SfxEffect::PrepareAudio(){

	  if (!m_audio){

		 m_audio = new AudioWrapper();
		 m_audio->InitializeXAudio();
		 m_audio->PrepareAudio(m_fileName);

	 }

 }

 const bool SfxEffect::isReady() const{

	 return m_audio->IsReady();
 }


 void SfxEffect::setToPlay(){

	 m_toPlay = true;
 }

 void SfxEffect::Update(float itime){

	 m_audio->UpdateAudio(itime);

 }

 const bool SfxEffect::isInPlay() const{

	 return m_audio->IsPlaing();
 }


 void SfxEffect::Play(){

	 if(m_audio->IsReady())
		m_audio->PlayAudio();

	  m_toPlay = false;

 }

 const bool SfxEffect::isToPlay()const{

	 return m_toPlay;

 }