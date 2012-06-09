#include "SfxEffect.h"


 const StringHash SfxEffect::SFX_EFFECT_ID = "SFX_EFFECT_ID";
 

 SfxEffect::SfxEffect(const ObjectId& objectId):ObjectProperty(SFX_EFFECT_ID,objectId),
												m_audio(NULL),m_toPlay(false),m_fileName(""),m_Loop(false)
 {


 }

 SfxEffect::~SfxEffect(){

	
	 MV_DELETE( m_audio);

 }
 
 void SfxEffect::setLoopActive(const bool iactive){

	 m_Loop = iactive;
 }

 void SfxEffect::setSound(char* const fileName){

	 m_fileName = fileName; 

 }
 
 void SfxEffect::PrepareAudio(){

	  if (!m_audio && m_fileName != "" ){

		 m_audio = MV_NEW Sound();
		 
		 m_audio->LoadSound(m_fileName);

		 m_audio->LoopSound(m_Loop);

	 }

 }

 const bool SfxEffect::isReady() const{

	 return m_audio->isReady();
 }


 void SfxEffect::setToPlay(){

	 m_toPlay = true;
 }

 void SfxEffect::Update(float itime){

	 m_audio->Update(itime);

 }

 const bool SfxEffect::isInPlay() const{

	 return m_audio->isPlaying();
 }


 void SfxEffect::Play(){

	 if(m_audio->isReady())
		m_audio->Play();

	  m_toPlay = false;

 }

 const bool SfxEffect::isToPlay()const{

	 return m_toPlay;

 }