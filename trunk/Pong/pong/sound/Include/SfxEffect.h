#pragma once
#include "ObjectProperty.h"
//#include "AudioWrapper.h"


class SfxEffect :public ObjectProperty{

public :

	static const StringHash SFX_EFFECT_ID;

	SfxEffect(const ObjectId& objectId):ObjectProperty(SFX_EFFECT_ID,objectId){}

	~SfxEffect(){}

	const bool isToPlay() const{return true;}

	void setToPlay(){}
	//const bool isToStop() const;
	void setSound(const LPCWSTR fileName){}

	void setDescription(std::string& desc){}

	void PrepareAudio(){}

	const bool isReady()const{return true;}

	void Play(){}

	void Update(float time){}

	const bool isInPlay() const{return true;}



private:

	bool m_toPlay;
	
	std::string m_description;

//	AudioWrapper* m_audio;

	LPCWSTR m_fileName;

};